#ifndef _geom_H
#define _geom_H

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


struct Triangle {
	GLint a;
	GLint b;
	GLint c;
};

//The TriangleMesh class actually stores the geometry of an object.
//It stores the vertices and a list of triangles (indices into the vertex list).
class TriangleMesh;

class TriangleMesh
{
	vector <glm::vec3> _vertices;         //_vertices stores the vertex positions as glm::vec4
	vector <glm::vec3> _norms;         //_norms stores the vertex normals
	vector <glm::vec2> _uvs;         //texture coordinates
	vector <Triangle> _triangles;         //_triangle stores the triangles as structs

	public:

	TriangleMesh(char * filename) {LoadFile(filename);};
	TriangleMesh() {};

	//Basic loading of vertices and faces (triangles) from .obj file
	void LoadFile(char * filename);
	void CalculateNormals();
	void CalculateUVSpherical();

	//Get the number of triangles in the mesh
	int TriangleCount() {
		return _triangles.size();
	};
	//Get the number of vertices in the mesh
	int VertexCount() {
		return _vertices.size();
	};

	std::vector<glm::vec3> &Vertices() {
		return _vertices;
	}
	std::vector<Triangle> &Triangles() {
		return _triangles;
	}
	std::vector<glm::vec3> &Norms() {
		return _norms;
	}
	std::vector<glm::vec2> &UVs() {
		return _uvs;
	}
};

//This function loads an obj format file
//This is a utility and should not have to be modified for bunny.obj (the assignment).
void TriangleMesh::LoadFile(char * filename)
{
	FILE *file = fopen(filename, "r");
	if (file  == NULL) {
		std::cout << "Can't open file " << filename << std::endl;
		return;
	}


	//Read in .obj
	while (1) {
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt" ) == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "f" ) == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3];
			//int matches = fscanf(file, "%d/%d %d/%d %d/%d\n",&vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
			int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			if (matches != 3) {
				std::cout << "Can't be read by simple parser!" << std::endl;
				return;
			}
			//indexed from 1
			Triangle t;
			t.a = vertexIndex[0] - 1;
			t.b = vertexIndex[1] - 1;
			t.c = vertexIndex[2] - 1;
			_triangles.push_back(t);
		}
	}

	cout << "Number of triangles: " << _triangles.size() << ", number of vertices: " << _vertices.size() << endl;
};

void TriangleMesh::CalculateNormals()
{
	//find triangles each vertex belongs to.
	//since the data is stored in obj format,
	//this is a slow step we need to perform once,
	//when we first load the data
	vector <vector <int> > vertTri;
	vertTri.resize(_vertices.size());
	for (int i = 0; i < _triangles.size(); i++)
	{
		Triangle vix = _triangles[i];
		//store this triangle as belonging to each vertex
		vertTri[vix.a].push_back(i);
		vertTri[vix.b].push_back(i);
		vertTri[vix.c].push_back(i);
	}
	//compute normal of each triangle
	vector <glm::vec3> triNorms;
	for (int i = 0; i < _triangles.size(); i++)
	{
		Triangle vix = _triangles[i];
		glm::vec3 u = _vertices[vix.b] - _vertices[vix.a];
		glm::vec3 v = _vertices[vix.c] - _vertices[vix.a];
		glm::vec3 trinorm = glm::cross(u, v);
		trinorm = glm::normalize(trinorm);
		triNorms.push_back(trinorm);
	}

	//compute normal of each vertex
	for (int i = 0; i < _vertices.size(); i++)
	{
		glm::vec3 norm;
		//calculate object normal vectors for each vertex here
		//using _triangles and _vertices
		norm.x = 0.0;
		norm.y = 0.0;
		norm.z = 0.0;
		for (int j = 0; j < vertTri[i].size(); j++)
		{
			norm = norm + triNorms[vertTri[i][j]];
		}
		norm = glm::normalize(norm);
		_norms.push_back(norm);
	}
}

#endif //_geom_H

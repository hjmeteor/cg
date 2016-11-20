#pragma once

#include "Ray.h"

//Holds material information of a particular object
class Material
{
public:
	Material();
	//Material values used for lighting equations
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularExponent;
	float reflection_index;
	float refraction_index;
	//TODO add further material values here such as reflection/refraction index
};


//Interface for an objet in the scene
class Object
{
public:
	//Constructor
	//@transform The transformation matrix for the object
	//@material The material properties of the object
	Object(const glm::mat4 &transform = glm::mat4(1.0f), const Material &material = Material());

	//Test whether a ray intersects the object
	//@ray The ray that we are testing for intersection
	//@info Object containing information on the intersection between the ray and the object(if any)
	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const { return true; }

	//Retrun the position of the object, according to its transformation matrix
	glm::vec3 Position() const { return glm::vec3(_transform[3][0],_transform[3][1],_transform[3][2]); }
	//Get a pointer to the objects material properties
	const Material *MaterialPtr() const { return &_material; }

protected:
	glm::mat4 _transform;
	Material _material;
};

//What I implement
class Sphere:public Object
{
public:
	Sphere(const glm::vec3 &c, const float &r):radius(r), radius2(r * r), center(c){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		float t = 0.0;
		glm::vec3 L = center - ray.origin;
		float L2 = glm::dot(L, L);
		float LdotD = glm::dot(L, ray.direction);
		if(LdotD < 0 and L2 > radius2)
			return false;
		float d2 = L2 - LdotD * LdotD;
		if(d2 > radius2)
			return false;
		float q = sqrt(radius2 - d2);
		if (L2 > radius2)
			t = LdotD - q;
		else
			t = LdotD + q;

	 	info.time = t;
		info.material = &_material;
		info.hitPoint = ray(t);
		info.normal = ray(t) - center;
		return true;
	}

	float radius, radius2;
	glm::vec3 center;
};

//What I implement
class Triangle:public Object
{
public:
	Triangle(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2):p0(p0), p1(p1), p2(p2){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		glm::vec3 e1 = p1 - p0;
		glm::vec3 e2 = p2 - p0;
		glm::vec3 n(glm::cross(e1, e2));

		float ndd = glm::dot(n, ray.direction);
		// check if ray and plane are parallel ?
		if (fabs(ndd) < 0.0001f){ // almost 0
			return false;
		}
		float d = glm::dot(n, p0);
		float t = -(glm::dot(n, ray.origin) + d) / ndd;
		// check if the triangle is in behind the ray
    	if (t < 0.0f){
			return false; // the triangle is behind
		}
		info.time = t;
		info.material = &_material;
		info.hitPoint = ray(t);
		info.normal = n;
		// Step 2: inside-outside test
	    glm::vec3 c; // vector perpendicular to triangle's plane

	    // edge 0
	    glm::vec3 edge0(p1 - p0);
	    glm::vec3 hit_p0(info.hitPoint - p0);
		c = glm::cross(edge0, hit_p0);
	    if (glm::dot(n, c) < 0.0f) return false; // P is on the right side

	    // edge 1
		glm::vec3 edge1(p2 - p1);
	    glm::vec3 hit_p1(info.hitPoint - p1);
		c = glm::cross(edge1, hit_p1);
	    if (glm::dot(n, c) < 0.0f) return false;// P is on the right side

	    // edge 2
		glm::vec3 edge2(p0 - p2);
	    glm::vec3 hit_p2(info.hitPoint - p2);
		c = glm::cross(edge2, hit_p2);
	    if (glm::dot(n, c) < 0.0f) return false; // P is on the right side;

	    return true; // this ray hits the triangle

		// glm::vec3 q = glm::cross(ray.direction , e2);
		// float a = glm::dot(e1, q);
		// // check if ray and plane are parallel ?
		// if(a > -0.0001f and a < 0.0001f) // almost 0
		// 	return false;
		// float f = 1.0f / a;
		// glm::vec3 s = ray.origin - p0;
		// // glm::vec3 s ;
		// // if(a > 0)
		// // 	 {s = ray.origin - p0;}
		// // else{
		// // 	s = p0 - ray.origin;
		// // 	a = -a;}
		// //
		// float u = f * (glm::dot(s, q));
		// if(u < 0.0f | u > 1.0f)
		//  	return false;
		// glm::vec3 r = glm::cross(s, e1);
		// float v = f * (glm::dot(ray.direction, r));
		// if(v < 0.0 or u + v > 1.0f)
		// 	return false;
		// float t = f * (glm::dot(e2, q));
		// info.time = t;
		// info.material = &_material;
		// info.hitPoint = ray(t);
		// info.normal = n;
		// return true;
	}
	glm::vec3 p0, p1, p2;
};

//What I implement
class Plane:public Object
{
public:
	Plane(const glm::vec3 &p0, const glm::vec3 &n):p0(glm::normalize(p0)),n(glm::normalize(n)){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		float np = glm::dot(n, p0);
		float nd = glm::dot(n, glm::normalize(ray.direction));
		if(nd < 1e-6) return false;
		float no = glm::dot(n, glm::normalize(ray.origin));
		float t = (np - no) / nd;
		if(t < 0.0f)
			return false;
		info.time = t;
		info.material = &_material;
		info.hitPoint = ray(t);
		info.normal = n;
		// info.normal = n;
		return true;
	}
	glm::vec3 p0, n;
};

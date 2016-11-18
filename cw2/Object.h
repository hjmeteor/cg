#pragma once

#include "Ray.h"
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
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

// houzi
// bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
// {
// 	float discr = b * b - 4 * a * c;
// 	if(discr < 0)
// 		return false;
// 	else if (discr == 0){
// 		x0 = x1 = -0.5 * b /a;
// 	}
// 	else{
// 		float q = (b > 0) ?
// 		-0.5 * (b + sqrt(discr)) :
// 		-0.5 * (b - sqrt(discr));
// 		x0 = q / a;
// 		x1 = c / q;
// 	}
// 	return true;
// }
// houzi
class Sphere:public Object
{
public:
	Sphere(const glm::vec3 &c, const float &r):radius(r), radius2(r * r), center(c){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		float t0, t1;
		glm::vec3 L = center - ray.origin;
		float tca = glm::dot(L, ray.direction);
		if(tca < 0)
			return false;
		float d2 = glm::dot(L,L) - tca * tca;
		if(d2 > radius2)
			return false;
		float thc = sqrt(radius2 - d2);
		t0 = tca - thc;
		t1 = tca - thc;

		L = ray.origin - center;
		float a = glm::dot(ray.direction, ray.direction);
		float b = 2 * glm::dot(ray.direction, L);
		float c = glm::dot(L,L) - radius2;
		if(!solveQuadratic(a, b, c, t0, t1))
			return false;
		if(t0 > t1)
		{
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}
		if(t0 < 0)
		{
			t0 = t1;
			if(t0 < 0)
				return false;
		}
		t1 = t0;
		return true;
	}
	float radius, radius2;
	glm::vec3 center;
};

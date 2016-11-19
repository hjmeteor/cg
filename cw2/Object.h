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
		info.normal = glm::normalize(ray(t) - center);
		return true;
	}
	
	float radius, radius2;
	glm::vec3 center;
};

class Triangle:public Object
{
public:
	Triangle(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2):p0(p0), p1(p1), p2(p2){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		glm::vec3 e1 = p1 - p0;
		glm::vec3 e2 = p2 - p0;
		glm::vec3 q = glm::cross(ray.direction , e2);
		float a = glm::dot(e1, q);
		if(a > -0.0001f and a < 0.0001f)
			return false;
		float f = 1 / a;
		glm::vec3 s = ray.origin - p0;
		// glm::vec3 s ;
		// if(a > 0)
		// 	 {s = ray.origin - p0;}
		// else{
		// 	s = p0 - ray.origin;
		// 	a = -a;}
		// 
		float u = f * (glm::dot(s, q));
		if(u < 0.0f | u > 1)
		 	return false;
		glm::vec3 r = glm::cross(s, e1);
		float v = f * (glm::dot(ray.direction, r));
		if(v < 0.0 or u + v > 1)
			return false;
		float t = f * (glm::dot(e2, q));
		glm::vec3 n(glm::cross(e1, e2));
		
		info.time = t;
		cout<<t;
		info.material = &_material;
		info.hitPoint = ray(t);
		info.normal = glm::normalize(n);
		info.normal = n;
		// return true;
	}
	glm::vec3 p0, p1, p2;
};

class Plane:public Object
{
public:
	Plane(const glm::vec3 &p0, const glm::vec3 &n):p0(p0),n(n){};
	bool Intersect(const Ray &ray, IntersectInfo &info) const
	{
		float np = glm::dot(n, p0);
		float nd = glm::dot(n, ray.direction);
		float no = glm::dot(n, ray.origin);
		float t = (np - no) / nd;
		if(t < 0)
			return false;	
		info.time = t;
		info.material = &_material;
		info.hitPoint = ray(t);
		info.normal = glm::normalize(n);
		// info.normal = n;
		return true;
	}
	glm::vec3 p0, n;
};




#pragma once

#include "header.h"
//What I done
//#include "Object.h"

class Material;
//forward declaration of Material class so we can include it in the IntersectInfo class
//class Material(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &, const glm::vec3 &, const float &, const float &, const float &);
//What I done
//{
//public:
//    //What I done
//    //Redefines the constructor declaration for class material
//    Material(const glm::vec3 &color = glm::vec3(1.0f), const glm::vec3 &ambient = glm::vec3(0.6f), const glm::vec3 &diffuse = glm::vec3(0.6f), const glm::vec3 &specular = glm::vec3(1.0f), const float &specularExponent = 10.0f, const float &reflection_index = 0.8f, const float &refraction_index = 0.5f);
//    //Material values used for lighting equations
//    //What I done
//    //add color value
//    glm::vec3 color;
//    glm::vec3 ambient;
//    glm::vec3 diffuse;
//    glm::vec3 specular;
//    float specularExponent;
//    //What I done
//    //add material values for reflection and refraction index
//    float reflection_index;
//    float refraction_index;
//    //TODO add further material values here such as reflection/refraction index
//};

//Defines a ray object
//A ray is defined by its origin and its normalised direction
class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(const glm::vec3 &origin, const glm::vec3 &direction):
	origin(origin),
	direction(direction)
	{
	}

	//Returns the position of the ray at time t i.e. the solution to: RayPosition = RayOrigin + time*RayDirection;
	//Usage: position = ray(t);
	glm::vec3 operator() (const float &t) const
	{
		return origin + direction*t;
	}
};

//Used to hold information on the intersection of a ray with an object in the scene
class IntersectInfo
{
public:
	IntersectInfo():
	  time(std::numeric_limits<float>::infinity()),
		hitPoint(0.0f),
		normal(0.0f),
		material(NULL)
	{		
	}

	//The position of the intersection in 3D coordinates
	glm::vec3 hitPoint;
	//The normal vector of the surface at the point of the intersection
	glm::vec3 normal;
	//The time along the ray that the intersection occurs
	float time;
	//The material of the object that was intersected
	const Material *material;

	//Basic assignment operator
	IntersectInfo &operator =(const IntersectInfo &rhs)
	{
		hitPoint = rhs.hitPoint;
		material = rhs.material;
		normal = rhs.normal;
		time = rhs.time;
		material = rhs.material;
		return *this;
	}
};

//Holds information about the current state of the ray
class Payload
{
public:
	Payload():
    color(0.0f),
	numBounces(0)
	{
	}
	glm::vec3 color;				// Accumulated color of this ray.
	int numBounces;		// Number of bounces this ray has made so far.
};

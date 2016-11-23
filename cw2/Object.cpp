#include "Object.h"

#include "header.h"

Material::Material(const glm::vec3 &color, const glm::vec3 &ambient, const glm::vec3 &diffuse , const glm::vec3 &specular, const float &specularExponent, const float &reflection_index, const float &refraction_index):
color(color),
ambient(ambient),//Default to white
diffuse(diffuse),//Default to white
specular(specular),//Default to white
specularExponent(specularExponent),//Used in lighting equation
reflection_index(reflection_index),
refraction_index(refraction_index)
{
}

//Constructor
//@transform The transformation matrix for the object
//@material The material properties of the object
Object::Object(const glm::mat4 &transform,const Material &material):
_transform(transform),
_material(material)
{
}

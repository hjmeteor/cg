#include "Object.h"

#include "header.h"

Material::Material():
ambient(0.6f),//Default to white
diffuse(0.6f),//Default to white
specular(1.0f),//Default to white
specularExponent(10.0f),//Used in lighting equation
reflection_index(0.8f),
refraction_index(0.5f)
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

#include "Object.h"

#include "header.h"

Material::Material():
ambient(1.0f),//Default to white
diffuse(1.0f),//Default to white
specular(1.0f),//Default to white
specularExponent(10.0f)//Used in lighting equation
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

#include "Material.hpp"

Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, GLfloat shininess)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

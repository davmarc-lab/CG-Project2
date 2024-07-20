#include "Material.hpp"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, const char* name) : name(name) {
    this->vectors.ambient = ambient;
    this->vectors.diffuse = diffuse;
    this->vectors.specular = specular;
    this->shininess = shininess;
}


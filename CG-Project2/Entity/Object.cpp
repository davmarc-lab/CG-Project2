#include "Object.hpp"

Object::Object(const vec3 pos, const vec3 scale, const vec3 rot) : id(0)
{
    this->model.position = pos;
    this->model.scale = scale;
    this->model.euler = rot;
}

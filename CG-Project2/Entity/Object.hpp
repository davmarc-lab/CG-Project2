#pragma once

#include "../Lib.hpp"

struct Model
{
    vec3 position;
    vec3 scale;
    vec3 euler;
};

class Object
{
    private:
        unsigned int id;

        Model model;

        int nvertex = 0;

    public:
        ~Object() {}

        Object() { this->id = 0; }

        Object(const vec3 pos, const vec3 scale, const vec3 rot); 

        unsigned int getId() { return this->id; }

        void setId(const unsigned int id) { this->id = id; }
};

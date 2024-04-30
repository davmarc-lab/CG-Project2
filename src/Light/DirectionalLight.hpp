#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{
    private:
        vec3 direction;

    public:
        DirectionalLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction);

        virtual void drawLight(Shader shader) override;
};

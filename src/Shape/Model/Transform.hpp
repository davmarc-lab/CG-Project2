#pragma once

#include "../../Lib.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
    private:
        mat4 model = mat4(1.0f);
        vec3 translate = vec3(0.0f);
        vec3 scale = vec3(1.0f);
        quat rotation = quat(1, 0, 0, 0);

    public:
        Transform() {}

        void setModelMatrix(mat4 mat) { this->model = mat4(mat); }

        mat4 getModelMatrix() { return this->model; }

        void applyTransofrmation(vec3 translateVector,
                vec3 scaleVector,
                vec3 rotationAxis,
                float rotationValue);
};

#pragma once

#include "../../Lib.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform {
private:
    Model model;

    mat4 modelMatrix = mat4(1.0);

public:
    Transform() {}

    Transform(const mat4 mat) { this->modelMatrix = mat4(mat); }

    inline vec3 getPosition() { return this->model.position; }

    inline void setPosition(const vec3 position) { this->model.position = position; }

    inline vec3 getScale() { return this->model.scale; }

    inline void setScale(const vec3 scale) { this->model.scale = scale; }

    inline vec3 getRotation() { return this->model.rotation; }

    inline void setRotation(const vec3 rotation) { this->model.rotation = rotation; }

    inline void setModelMatrix(const mat4 mat) { this->modelMatrix = mat4(mat); }

    inline mat4 getModelMatrix() { return this->modelMatrix; }

    inline mat4 refreshModelMatrix() {
        mat4 model = mat4(1);
        mat4 trans = translate(model, this->getPosition());
        mat4 sca = scale(model, this->getScale());
        quat q = quat(this->getRotation());
        mat4 rot = toMat4(q);
        return trans * sca * rot;
    }

    inline void applyTransofrmation(vec3 translateVector, vec3 scaleVector, vec3 rotationAxis, float rotationValue) {
        mat4 base = mat4(1.0f);
        mat4 translate(1.0f), scale(1.0f), rotate(1.0f);
        quat quaternion;

        translate = glm::translate(base, translateVector);
        scale = glm::scale(base, scaleVector);

        if (rotationAxis == vec3(0)) {
            rotationAxis = vec3(1);
            rotationValue = 0;
        }

        quaternion = angleAxis(radians(rotationValue), rotationAxis);
        rotate = toMat4(quaternion);

        this->setModelMatrix(this->modelMatrix * translate * rotate * scale);
    }

    ~Transform() = default;
};

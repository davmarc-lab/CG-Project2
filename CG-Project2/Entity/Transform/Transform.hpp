#pragma once

#include "../../Lib.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform {
  private:
    Model model;

    mat4 modelMatrix = mat4(1.0);

    bool is_changed = false;

    inline void updateModelMatrix() {
        mat4 model = mat4(1);
        mat4 trans = translate(model, this->getPosition());
        mat4 sca = scale(model, this->getScale());
        quat q = quat(this->getRotation());
        mat4 rot = toMat4(q);
        this->modelMatrix = trans * sca * rot;
    }

  public:
    Transform() {}

    Transform(const mat4 mat) { this->modelMatrix = mat4(mat); }

    inline vec3 getPosition() { return this->model.position; }

    inline void setPosition(const vec3 position) {
        this->is_changed = true;
        this->model.position = position;
    }

    inline vec3 getScale() { return this->model.scale; }

    inline void setScale(const vec3 scale) {
        this->is_changed = true;
        this->model.scale = scale;
    }

    inline vec3 getRotation() { return this->model.rotation; }

    inline void setRotation(const vec3 rotation) {
        this->is_changed = true;
        this->model.rotation = rotation;
    }

    inline void setModelMatrix(const mat4 mat) { this->modelMatrix = mat4(mat); }

    inline mat4 getModelMatrix() {
        if (this->is_changed) {
            this->updateModelMatrix();
        }

        return this->modelMatrix;
    }

    ~Transform() = default;
};

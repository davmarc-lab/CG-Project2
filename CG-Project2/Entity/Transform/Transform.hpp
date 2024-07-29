#pragma once

#include "../../Lib.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/*
 * This class provides features to modify Entity shape, like translating, scaling and rotating.
 */
class Transform {
  private:
    // Stores all position, scale and rotation data.
    Model model;

    mat4 modelMatrix = mat4(1.0);

    // Flag used to tell if position or scale or rotation is changed.
    bool is_changed = false;

    mat4 m_base = mat4(1);
    mat4 m_trans = mat4(1);
    mat4 m_sca = mat4(1);
    // Using quaternion to calculate the rotation matrix.
    quat m_q = quat(vec3(0));
    mat4 m_rot = mat4(1);

    /*
     * This method creates the model matrix from position, scale and rotation information.
     * It uses quaternion to calculate the rotation matrix, but the users uses Euler Angles to interact.
     * This feature could be useful to implement multiple rotation at the same time.
     */
    inline void updateModelMatrix() {
        this->m_trans = translate(this->m_base, this->getPosition());
        this->m_sca = scale(this->m_base, this->getScale());
        this->m_q = quat(this->getRotation());
        this->m_rot = toMat4(this->m_q);
        this->modelMatrix = this->m_trans * this->m_sca * this->m_rot;
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

    // Retrieves the model matrix, if some information are changed.
    inline mat4 getModelMatrix() {
        if (this->is_changed) {
            this->updateModelMatrix();
            this->is_changed = false;
        }
        return this->modelMatrix;
    }

    ~Transform() = default;
};

#pragma once

#include "IComponent.hpp"

#include "../Lib.hpp"

class Rotation {
    private:
        vec3 rotationAxis = vec3(0);
        float rotationAngle = 0.0f;
        quat rotationQuaternion;
        mat4 rotationMatrix = mat4(1.0f);

        void updateQuaternion() { rotationQuaternion = glm::angleAxis(rotationAngle, rotationAxis); }

    public:
        Rotation() { updateQuaternion(); }

        Rotation(vec3 rotation) {
            // initialize quaternion
            updateQuaternion();

            // exec my rotation
            const vec3 axis = glm::normalize(abs(rotation));
            if (rotation.x != 0) {
                this->setRotation(vec3(axis.x, 0, 0), rotation.x);
                this->rotateObject();
            }
            if (rotation.y != 0) {
                this->setRotation(vec3(0, axis.y, 0), rotation.y);
                this->rotateObject();
            }
            if (rotation.z != 0) {
                this->setRotation(vec3(0, 0, axis.z), rotation.z);
                this->rotateObject();
            }
        }

        void setRotation(const vec3 &axis, float angleDegrees) {
            rotationAxis = normalize(axis);
            rotationAngle = radians(angleDegrees);
            updateQuaternion();
        }

        // You can remove args
        mat4 rotateObject() {
            mat4 rotationMatrix = glm::mat4_cast(rotationQuaternion);
            this->rotationMatrix = rotationMatrix * this->rotationMatrix;
            return this->rotationMatrix;
        }
};

struct Model {
    vec3 position = vec3();
    vec3 scale = vec3();
    vec3 rotation = vec3();
    Rotation rot;
};

class Transform : public IComponent {
    private:
        Model info = {};
        mat4 model = mat4(1.0f);

    public:
        Transform(vec3 position, vec3 scale, vec3 rotation) : info{position, scale, rotation, Rotation(rotation)} {}

        Transform() {}

        void applyTransform() {
            mat4 base = mat4(1.0f);
            mat4 translate = glm::translate(base, this->info.position);
            mat4 scale = glm::scale(base, this->info.scale);
            mat4 rotation = this->info.rot.rotateObject();

            this->model = this->model * translate * scale * rotation;

            // implementig rotation using quaternions
        }

        const vec3 getPosition() { return this->info.position; }

        void setPosition(vec3 position) { this->info.position = position; }

        const vec3 getScale() { return this->info.scale; }

        /* const vec3 getRotation() { return this->info.rotation; } */
};

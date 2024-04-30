#include "Transform.hpp"

void Transform::applyTransofrmation(vec3 translateVector, vec3 scaleVector,
                                    vec3 rotationAxis, float rotationValue) {
    mat4 base = mat4(1.0f);
    mat4 translate(1.0f), scale(1.0f), rotate(1.0f);

    translate = glm::translate(base, translateVector);
    scale = glm::scale(base, scaleVector);

    if (rotationAxis == vec3(0)) {
        rotationAxis = vec3(1);
        rotationValue = 0;
    }

    this->rotation = angleAxis(radians(rotationValue), rotationAxis);
    rotate = toMat4(this->rotation);

    this->setModelMatrix(this->model * translate * rotate * scale);
}

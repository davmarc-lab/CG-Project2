#pragma once

#include "../Color/Color.hpp"
#include "Collider/CubeCollider.hpp"
#include "Entity.hpp"

class CubeEntity : public Entity {
private:
    void buildCube();

    Color color = color::BLACK;

public:
    CubeEntity() {
        buildCube();
        this->nvertex = this->coords.vertex.size();
    }

    inline virtual void applyTransformation(const vec3 translate, const vec3 scale, const vec3 rotAxis, float rotValue) override {
        this->setPosition(this->getPosition() + translate);
        this->setScale(scale);
        this->setRotation(this->getRotation() + rotAxis * rotValue);
        this->transform.applyTransofrmation(translate, scale, rotAxis, rotValue);
    }

    inline void setCollidable() {
        this->collider = new CubeCollider(&this->transform);
        ((CubeCollider*)this->collider)->setBoundingBox(this->getMinVertex(), this->getMaxVertex());
    }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~CubeEntity() = default;
};

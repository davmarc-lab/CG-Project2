#pragma once

#include "Entity.hpp"
#include "../Color/Color.hpp"

class PlaneEntity : public Entity {
private:
    Color color;

    void buildPlane();

public:

    PlaneEntity() : PlaneEntity(color::BLACK) {
    }

    PlaneEntity(Color color) {
        this->color = color;
        this->buildPlane();
    }

    virtual void applyTransformation(const vec3 translate, const vec3 scale, const vec3 rotAxis, float rotValue) override {
    }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~PlaneEntity() = default;
};

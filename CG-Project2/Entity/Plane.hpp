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

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~PlaneEntity() = default;
};

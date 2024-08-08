#pragma once

#include "../Color/Color.hpp"
#include "Entity.hpp"

class PlaneEntity : public Entity {
  private:
    Color color;

    void buildPlane();

  public:
    PlaneEntity() : PlaneEntity(color::GRAY) {}

    PlaneEntity(Color color) {
        this->color = color;
        this->buildPlane();
    }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~PlaneEntity() = default;
};

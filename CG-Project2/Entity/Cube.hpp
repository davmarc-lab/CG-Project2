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

    inline void setCollidable() {
        this->collider = new CubeCollider(&this->transform);
        ((CubeCollider*)this->collider)->setBoundingBox(this->getMinVertex(), this->getMaxVertex());
    }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~CubeEntity() = default;
};

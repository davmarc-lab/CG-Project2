#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "Shape3D.hpp"

#include <vector>

class Cube : public Shape3D
{
private:
    GLuint ebo;
    vector<int> indices;

    void buildCube();

public:
    Cube() {
        buildCube();
        this->setVertexNum(this->vertex.size());
    }

    Cube(Color color);

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    virtual void clearShape() override;

    ~Cube() = default;
};

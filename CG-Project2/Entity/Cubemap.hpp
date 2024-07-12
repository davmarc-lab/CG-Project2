#pragma once

#include "../Lib.hpp"
#include "Entity.hpp"

#include <string>

class Cubemap : public Entity {
private:
    string path;
    GLuint id;

public:
    Cubemap() {}

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~Cubemap() = default;
};

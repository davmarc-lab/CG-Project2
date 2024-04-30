#pragma once

#include "../Lib.hpp"
#include "Shape3D.hpp"

#include <string>
#include <vector>

class Cubemap : public Shape3D
{
    private:
        string path;
        GLuint id;

    public:
        Cubemap() {}

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void clearShape() override;
};

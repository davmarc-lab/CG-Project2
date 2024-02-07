#pragma once

#include "IComponent.hpp"

#include "../Buffers/VBO.hpp"
#include "../Shader/Shader.hpp"

#include "../Lib.hpp"

struct Buffers {
    VBO vbo_g;
    VBO vbo_c;
};

class Render : public IComponent {
    private:
        unsigned int ntriangles = 0;
        Buffers buffers;

    public:
        Render() {}

        Render(vector<vec3> vertex, vector<vec4> color, int ntriangles) : ntriangles(ntriangles) {
            // vertex vbo
            this->buffers.vbo_g = VBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
            GLuint id = this->buffers.vbo_g.bind();
            this->buffers.vbo_g.bufferData(sizeof(vec3) * vertex.size(), vertex.data());
            this->buffers.vbo_g.bindAttribPosition(0, 0, 3, 0, (void*)0);

            // color vbo
            this->buffers.vbo_c = VBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
            id = this->buffers.vbo_c.bind();
            this->buffers.vbo_c.bufferData(sizeof(vec4) * color.size(), color.data());
            this->buffers.vbo_c.bindAttribPosition(1, 1, 4, 0, (void*)0);
        }

        ~Render() = default;
};

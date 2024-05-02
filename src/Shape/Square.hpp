#pragma once

#include "../Lib.hpp"

#include "../Shader/Shader.hpp"
#include "../Color/Color.hpp"

#include <vector>

/*
 * This class creates a square shape implementing ComplexShape2D interface.
 */
class Square
{
private:

    vector<vec3> vertex;
    vector<vec4> colors;
    int nvertex;
    GLuint ebo;
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;

    // square vertices
    float vertices[12] = {
        -1.0f, -1.0f, 0.0f,		// bottom left
        -1.0f, 1.0f, 0.0f,		// top left
        1.0f, 1.0f, 0.0f,		// top right
        1.0f, -1.0f, 0.0f		// bottom right
    };
    // indices for the ebo buffer
    unsigned int indices[6] = { 0, 1, 3, 1 ,2 ,3 };

public:
    // Constructs a square with a given color.
    Square(Color color) {
        for (int i = 0; i < 12; i += 3)
            this->vertex.push_back(vec3(vertices[i], vertices[i+1], vertices[i+2]));

        for (int i = 0; i < 4; i++)
        {
            this->colors.push_back(color.getColorVector());
        }
        this->nvertex = 6;
    }

    inline void createVertexArray() {
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo_g);
        glGenBuffers(1, &this->vbo_c);
        glGenBuffers(1, &this->ebo);

        glBindVertexArray(this->vao);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
        glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);
    }

    inline void draw(Shader shader) {
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->nvertex, GL_UNSIGNED_INT, 0);
    }

    ~Square() = default;
};

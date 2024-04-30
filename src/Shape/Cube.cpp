#include "Cube.hpp"
#include <glm/trigonometric.hpp>

void Cube::buildCube()
{
    float vertices[] = {
        // vertex coord,   texture coord,  normal vectors
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    int size = sizeof(vertices) / sizeof(float);
    for (int i = 0; i < size; i+=8)
    {
        this->vertex.push_back(vec3(vertices[i], vertices[i+1], vertices[i+2]));
        this->texCoord.push_back(vec2(vertices[i+3], vertices[i+4]));
        this->normal.push_back(vec3(vertices[i+5], vertices[i+6], vertices[i+7]));
        this->colors.push_back(this->color.getColorVector());
    }
}

Cube::Cube(Color color)
{
    this->color = color;
    buildCube();
    this->setVertexNum(this->vertex.size());
}

void Cube::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);

    // vbo geometry
    glGenBuffers(1, &this->vbo_g);
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(vec3), this->vertex.data(),  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // vbo colors
    glGenBuffers(1, &this->vbo_c);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // vbo normals
    glGenBuffers(1, &this->vbo_n);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_n);
    glBufferData(GL_ARRAY_BUFFER, this->normal.size() * sizeof(vec3), this->normal.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);
    
}

void Cube::draw(Shader shader)
{
    auto modelLoc = glGetUniformLocation(shader.getId(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(this->transform.getModelMatrix()));

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
}

void Cube::clearShape()
{
    this->clearVertexArray();
    this->clearColorArray();
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo_g);
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo_c);
}


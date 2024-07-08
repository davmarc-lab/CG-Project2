#pragma once

#include "../Lib.hpp"

#include "../Shape/Cube.hpp"

class Crosshair {

private:
    Shape3D* square;
    Shader shader;
    mat4 first = mat4(1.f), second = mat4(1.f);
public:
    Crosshair() {}

    inline void initializeCrosshair() {
        shader = Shader("./resources/shaders/crosshairVertexShader.glsl", "./resources/shaders/crosshairFragmentShader.glsl");
        shader.use();
        shader.setMat4("projection", projection);

        this->square = new Cube(color::WHITE);
        this->square->createVertexArray();
        this->square->transformMesh(vec3(0), vec3(0.003, 0.026, 0), vec3(0), 0);
        first = this->square->getModelMatrix();
        this->square->setModelMatrix(mat4(1.f));
        this->square->transformMesh(vec3(0), vec3(0.0158, 0.004, 0), vec3(0), 0);
        second = this->square->getModelMatrix();
        
    }

    inline void drawCrosshair(mat4 view) {
        shader.use();
        shader.setMat4("view", view);
        this->square->setModelMatrix(first);
        this->square->draw(shader);
        this->square->setModelMatrix(second);
        this->square->draw(shader);
    }

    ~Crosshair() = default;
};

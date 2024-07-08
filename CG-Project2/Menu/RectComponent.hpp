#pragma once

#include "Component.hpp"

class RectComponent : public Component {
private:
    float vertices[12] = {
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f,  0.0f, // top left
    1.0f,  1.0f,  0.0f, // top right
    1.0f,  -1.0f, 0.0f  // bottom right
    };

    unsigned int indices[6] = {0, 1, 3, 1, 2, 3};
    int nvertex = 6;
public:
    RectComponent() : Component() {}

    RectComponent(Color color) : Component() {
        for (int i = 0; i < 4; i++)
        {
            this->coords.colors.push_back(color.getColorVector());
        }
    }

    inline void createVertexArray() override {
        glGenVertexArrays(1, &this->buffers.vao);
        glGenBuffers(1, &this->buffers.vbo_g);
        glGenBuffers(1, &this->buffers.vbo_c);
        glGenBuffers(1, &this->buffers.ebo);

        glBindVertexArray(this->buffers.vao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_g);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_c);
        glBufferData(GL_ARRAY_BUFFER, this->coords.colors.size() * sizeof(vec4), this->coords.colors.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);
    }

    inline void draw() override {
        this->render.shader.use();
        this->render.shader.setMat4("model", this->getModelMatrix());
        glBindVertexArray(this->buffers.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // render addons
        // --- Text ---
        if (this->addons.labels.size() > 0) {
            // create Shader for text
            Shader textShader = Shader("./resources/shaders/textVertexShader.glsl", "./resources/shaders/textFragmentShader.glsl");
            for (Text* elem: this->addons.labels) {
                elem->renderText(textShader);
            }
        }

    }

    inline void refresh() override {
    }

    inline void clear() override {
    }

    ~RectComponent() = default;
};

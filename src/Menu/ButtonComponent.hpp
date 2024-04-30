#pragma once

#include "Component.hpp"

class ButtonComponent : public Component {
private:
    float vertices[12] = {
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f, 1.0f,  0.0f, // top left
        1.0f,  1.0f,  0.0f, // top right
        1.0f,  -1.0f, 0.0f  // bottom right
    };

    unsigned int indices[6] = {0, 1, 3, 1, 2, 3};
    int nvertex = 6;

    Text *label;

public:
    ButtonComponent() : Component() {
        label = new Text("No Label", 20);
        this->addons.labels.push_back(label);
    }

    ButtonComponent(vec3 buttonPos, Color color, string text,
                    unsigned int textSize, Color textColor)
        : Component() {
        this->color.bgColor = color;
        this->color.fgColor = textColor;

        for (int i = 0; i < 4; i++) {
            this->coords.colors.push_back(color.getColorVector());
        }

        // text
        this->label = new Text(text, textSize);
        this->label->setColor(this->getFrontColor());
        this->setPosition(buttonPos);
        this->addLabel(label);
    }

    inline void setFontSize(const unsigned int size) {
        this->label->setFontSize(size);
    }

    inline void setText(string text) { this->label->setText(text); }

    inline void appendText(string text) { this->label->appendText(text); }

    inline Color getLabelColor() { return this->label->getColorValues(); }

    inline void setLabelColor(Color color) { this->label->setColor(color); }

    inline bool isMouseColliding(const vec2 mousePos) {
        vec3 topRight = vec3(this->getModelMatrix() * vec4(1));
        vec3 botLeft = vec3(this->getModelMatrix() * vec4(-1, -1, -1, 1));

        bool collisionX = mousePos.x <= topRight.x && mousePos.x >= botLeft.x;
        bool collisionY = mousePos.y <= topRight.y && mousePos.y >= botLeft.y;
        return collisionX && collisionY;
    }

    inline void createVertexArray() override {
        glGenVertexArrays(1, &this->buffers.vao);
        glGenBuffers(1, &this->buffers.vbo_g);
        glGenBuffers(1, &this->buffers.vbo_c);
        glGenBuffers(1, &this->buffers.ebo);

        glBindVertexArray(this->buffers.vao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_g);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices),
                     this->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_c);
        glBufferData(GL_ARRAY_BUFFER, this->coords.colors.size() * sizeof(vec4),
                     this->coords.colors.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);

        this->label->createVertexArray();
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
            Shader textShader =
                Shader("./resources/shaders/textVertexShader.glsl",
                       "./resources/shaders/textFragmentShader.glsl");
            for (Text *elem : this->addons.labels) {
                elem->renderText(textShader);
            }
        }
    }

    inline void refresh() override {
        // refresh colors buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_c);
        glBufferData(GL_ARRAY_BUFFER, this->coords.colors.size() * sizeof(vec4),
                     this->coords.colors.data(), GL_STATIC_DRAW);
    }

    inline void clear() override {
        this->label->clear();

        for (const auto elem: this->addons.labels) {
            elem->clear();
        }

        // clear all buffers
        glDeleteVertexArrays(1, &this->buffers.vao);
        glDeleteBuffers(1, &this->buffers.vbo_g);
        glDeleteBuffers(1, &this->buffers.vbo_c);
        glDeleteBuffers(1, &this->buffers.ebo);
    }

    ~ButtonComponent() = default;
};

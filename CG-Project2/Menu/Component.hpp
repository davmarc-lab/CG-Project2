#pragma once

#include "../Lib.hpp"

#include "../Color/Color.hpp"
#include "../Shader/Shader.hpp"
#include "../Text/Text.hpp"
#include <vector>

struct Colors {
    Color bgColor = color::WHITE;
    Color fgColor = color::BLACK;
};

struct Render {
    Shader shader = Shader("./resources/shaders/menuVertexShader.glsl",
                           "./resources/shaders/menuFragmentShader.glsl");
    mat4 projection = mat4(1.0f);
};

struct Addon {
    vector<Text *> labels;
};

class Component {
public:
    Buffers buffers;
    Coords coords;
    Model model;
    Colors color;
    Render render;
    Addon addons;

    Component() {
        this->render.shader.use();
        this->render.shader.setMat4("projection", textProjection);
    }

    Component(vector<vec3> vertex, vector<vec4> colors)
    : coords({vertex, colors}) {
        this->render.shader.use();
        this->render.shader.setMat4("projection", textProjection);
    }

    Component(vector<vec3> vertex, vector<vec4> colors, vec3 pos, vec3 scale)
    : coords({vertex, colors}), model({pos, scale}) {
        this->render.shader.use();
        this->render.shader.setMat4("projection", textProjection);
    }

    inline vector<vec3> getVertex() { return this->coords.vertex; }

    inline vector<vec4> getColors() { return this->coords.colors; }

    inline vec3 getPosition() { return this->model.position; }

    inline void setPosition(const vec3 pos) { this->model.position = pos; }

    inline vec3 getScale() { return this->model.scale; }

    inline void setScale(const vec3 scale) { this->model.scale = scale; }

    inline mat4 getModelMatrix() {
        const mat4 base = mat4(1.0f);
        return base * translate(base, this->model.position) *
            scale(base, this->model.scale);
    }

    inline Color getBackColor() { return this->color.bgColor; }

    inline void updateCoordsColor(Color color) {
        auto size = this->coords.colors.size();
        this->coords.colors.clear();

        for (int i = 0; i < size; i++) {
            this->coords.colors.push_back(color.getColorVector());
        }
    }

    inline void setBackColor(const Color color) {
        this->color.bgColor = color;
        this->updateCoordsColor(color);
    }

    inline Color getFrontColor() { return this->color.fgColor; }

    inline void setFrontColor(const Color color) {
        this->color.fgColor = color;
    }

    /*
    * This method takes all labels of a Component and re-center them.
    */
    inline void refreshLabelCenter() {
        for (const auto elem : this->addons.labels) {
            auto center = this->getPosition();
            vec3 textSize = vec3(elem->getTotalWidth(), elem->getTotalHeight(), 0);

            elem->setPosition(abs(center - vec3(textSize.x / 2, textSize.y / 2, 0)));
        }
    }

    /*
     * This method add a Text to a Component in the center.
     * You can modify the position of the Text after adding.
     */
    inline void addLabel(Text *text) {
        this->addons.labels.push_back(text);
        this->refreshLabelCenter();
    }

    virtual void createVertexArray() = 0;

    virtual void draw() = 0;

    virtual void clear() = 0;

    virtual void refresh() = 0;

    ~Component() = default;
};

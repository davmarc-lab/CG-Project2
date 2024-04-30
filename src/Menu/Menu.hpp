#pragma once

#include <vector>

#include "Component.hpp"

enum MenuState {
    INIT,
    RENDER,
    PLAY,
    PAUSE,
    CLOSE,
    EXIT,
    NONE
};

class Menu {

public:
    vector<Component*> components;
    MenuState state = NONE;
    GLFWwindow* window;

    Menu(GLFWwindow* window) {
        this->window = window;
        this->state = INIT;
    }

    inline void addComponent(Component* component) { this->components.push_back(component); }

    inline MenuState getState() { return this->state; }

    inline void setState(const MenuState state) { this->state = state; }

    virtual void processMouseInput(const float dt) = 0;

    virtual void update(const float dt) = 0;

    inline void render() {
        for (auto elem: this->components) {
            elem->draw();
        }
    }

    virtual void close() = 0;

    virtual void exit() = 0;

    virtual void clear() = 0;

    ~Menu() = default;
};

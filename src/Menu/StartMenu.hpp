#pragma once

#include "ButtonComponent.hpp"
#include "Menu.hpp"
#include "OptionPane.hpp"
#include <GLFW/glfw3.h>

class StartMenu : public Menu {

private:
    OptionPane* op;
    ButtonComponent *playButton = new ButtonComponent(
        vec3(WIDTH / 2, HEIGHT / 2, 0),
        color::WHITE,
        "Play",
        40,
        color::BLACK
    );
    
    ButtonComponent *optionButton = new ButtonComponent(
        vec3(WIDTH / 2, HEIGHT / 2 - 100, 0),
        color::WHITE,
        "Option",
        40,
        color::BLACK
    );

    ButtonComponent *exitButton = new ButtonComponent(
        vec3(WIDTH / 2, HEIGHT / 2 - 200, 0),
        color::WHITE,
        "Exit",
        40,
        color::BLACK
    );

    bool showOption = false;

    inline void initializeMenu() {
        this->playButton->setScale(vec3(100, 30, 0));
        this->playButton->createVertexArray();
        this->optionButton->setScale(vec3(100, 30, 0));
        this->optionButton->createVertexArray();
        this->exitButton->setScale(vec3(100, 30, 0));
        this->exitButton->createVertexArray();

        this->addComponent(playButton);
        this->addComponent(optionButton);
        this->addComponent(exitButton);
    }

public:
    StartMenu(GLFWwindow *window) : Menu(window) {
        this->initializeMenu();
    }

    bool isMouseInButton(ButtonComponent *button, vec2 pos) {
        return button->isMouseColliding(pos);
    }

    void showOptionMenu() {
        this->components.clear();
        op = new OptionPane();
        for (const auto elem: op->getElements()) {
            this->components.push_back(elem);
        }
    }

    GLenum oldState = GL_FALSE;

    inline void processMouseInput(const float dt) override {
        double x, y;
        glfwGetCursorPos(this->window, &x, &y);
        y = HEIGHT - y;
        vec2 pos = vec2(x, y);

        // mouse left click actions
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS) {
            oldState = state;
        }
        // do these actions if the option pane is active
        if (oldState == GLFW_PRESS && state == GLFW_RELEASE) {
            if (this->showOption) {
                bool res = this->op->execOperation(pos);
                if (res) {
                    this->initializeMenu();
                    this->showOption = false;
                }
            } else {
                // do these action if the option pane is not active
                if (isMouseInButton(this->playButton, pos)) {
                    this->state = PLAY;
                } else if (isMouseInButton(this->exitButton, pos)) {
                    this->exit();
                } else if (isMouseInButton(this->optionButton, pos)) {
                    this->showOption = true;
                    showOptionMenu();
                }
            }
            oldState = GL_FALSE;
        }
    }

    inline void update(const float dt) override {
    }

    inline void close() override { }

    inline void exit() override {
        // clear menu
        this->state = EXIT;
        this->components.clear();
    }

    inline void clear() override {
        this->playButton->clear();
        this->exitButton->clear();
    }

    ~StartMenu() = default;
};

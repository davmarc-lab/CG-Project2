#include "Game.hpp"
#include "GameState.hpp"

#include "../Window/Window.hpp"

Mouse input;

GameEngine::GameEngine(Window *window, unsigned int width, unsigned int height) {
    this->window = window;
    this->width = width;
    this->height = height;
}

void mouseMovementCallback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (input.first_mouse) {
        input.first_mouse = false;
        input.lastX = xpos;
        input.lastY = ypos;
    }

    float xoffset = xpos - input.lastX;
    float yoffset = ypos - input.lastY;

    input.lastX = xpos;
    input.lastY = ypos;
}

void GameEngine::init() { this->running = true; }

void GameEngine::changeState(GameState *state) {
    // empty state buffer
    if (!this->states.empty()) {
        this->states.back()->clean();
    }

    this->states.push_back(state);
    this->states.back()->init();
}

void GameEngine::processInput() { this->states.back()->handleEvent(this); }

void GameEngine::update() { this->states.back()->update(this); }

void GameEngine::render() {
    // draw the game scene
    this->states.back()->draw(this);
}

void GameEngine::clean() {}

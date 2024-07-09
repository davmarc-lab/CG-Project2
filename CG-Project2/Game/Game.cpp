#include "Game.hpp"
#include "GameState.hpp"

#include "../Window/Window.hpp"
#include "PlayState.hpp"

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

    // camera.processMouseMovement(xoffset, yoffset);
}

void GameEngine::init() { this->running = true; }

void GameEngine::changeState(GameState *state) {
    // empty state buffer
    if (!this->states.empty()) {
        this->states.back()->clean();
    }

    // when loading play state set mouse movement callback
    if (state == PlayState::instance()) {
        // glfwSetInputMode(this->window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetCursorPosCallback(this->window->getWindow(), mouseMovementCallback);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(this->window->getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        glfwSetCursor(this->window->getWindow(), cursor);
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

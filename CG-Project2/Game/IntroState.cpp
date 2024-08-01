#include "IntroState.hpp"
#include "OptionState.hpp"
#include "Game.hpp"

#include "../Menu/ButtonComponent.hpp"
#include "PlayState.hpp"

#include "../Menu/Logger/LogManager.hpp"

IntroState IntroState::introState;

ButtonComponent *playButton, *optionButton, *exitButton;
GLenum oldState = GL_FALSE;
inline GLFWwindow* window_context;

// Creates custom button.
void IntroState::init() {
    playButton = new ButtonComponent(vec3(WIDTH / 2, HEIGHT / 2, 0), color::WHITE, "Play", 40, color::BLACK);
    optionButton = new ButtonComponent(vec3(WIDTH / 2, HEIGHT / 2 - 100, 0), color::WHITE, "Option", 40, color::BLACK);
    exitButton = new ButtonComponent(vec3(WIDTH / 2, HEIGHT / 2 - 200, 0), color::WHITE, "Exit", 40, color::BLACK);

    playButton->setScale(vec3(100, 30, 0));
    playButton->createVertexArray();

    optionButton->setScale(vec3(100, 30, 0));
    optionButton->createVertexArray();

    exitButton->setScale(vec3(100, 30, 0));
    exitButton->createVertexArray();
}

void IntroState::clean() {}

void IntroState::pause() { cout << "-- WARNING -- Cannot pause an intro" << endl; }

void IntroState::resume() { cout << "-- WARNING -- Cannot resume an intro" << endl; }

bool isMouseInButton(ButtonComponent* button, vec2 pos) {
    return button->isMouseColliding(pos);
}

// Manages input for the buttons, getting the cursor position.
void IntroState::handleEvent(GameEngine *engine) {
    window_context = engine->getWindow()->getGLFWContext();
    double x, y;
    glfwGetCursorPos(window_context, &x, &y);
    y = HEIGHT - y;
    vec2 pos = vec2(x, y);

    // mouse left click actions
    int state = glfwGetMouseButton(window_context, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        oldState = state;
    }

    // do these actions if the option pane is active
    if (oldState == GLFW_PRESS && state == GLFW_RELEASE) {
        // do these action if the option pane is not active
        if (isMouseInButton(playButton, pos)) {
            LogManager::instance()->addLog(logs::STATE, "Creating PlayState");
            engine->changeState(PlayState::instance());
            LogManager::instance()->addLog(logs::STATE, "Instanced PlayState");
        } else if (isMouseInButton(exitButton, pos)) {
            engine->quit();
        } else if (isMouseInButton(optionButton, pos)) {
            engine->changeState(OptionState::instance());
        }
        oldState = GL_FALSE;
    }
}

void IntroState::update(GameEngine *engine) {}

void IntroState::draw(GameEngine *engine) {
    playButton->draw();
    optionButton->draw();
    exitButton->draw();
}

#include "OptionState.hpp"

#include "../Menu/ButtonComponent.hpp"
#include "IntroState.hpp"

OptionState OptionState::optionState;

enum LightModel { PHONG, BLINN_PHONG, STANDARD };

ButtonComponent *backButton, *switchLight;

vector<pair<LightModel, string>> lightModels = {
    pair<LightModel, string>(LightModel::PHONG, "Phong"),
    pair<LightModel, string>(LightModel::BLINN_PHONG, "Blinn-Phong"),
    pair<LightModel, string>(LightModel::STANDARD, "Standard"),
};
unsigned int modelSelector = 2;

void OptionState::init() {

    backButton = new ButtonComponent(vec3(55, HEIGHT - 35, 0), color::WHITE, "Back", 40, color::BLACK);
    switchLight = new ButtonComponent(vec3(WIDTH / 2, HEIGHT / 2 + 200, 0), color::WHITE, lightModels[modelSelector].second, 30, color::BLACK);

    backButton->setScale(vec3(50, 30, 0));
    backButton->createVertexArray();
    switchLight->setScale(vec3(150, 20, 0));
    switchLight->createVertexArray();
}

void OptionState::clean() {}

void OptionState::pause() { cout << "-- WARNING -- Cannot pause an intro" << endl; }

void OptionState::resume() { cout << "-- WARNING -- Cannot resume an intro" << endl; }

void OptionState::handleEvent(GameEngine *engine) {
    auto window = engine->getWindow()->getWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    y = HEIGHT - y;
    vec2 pos = vec2(x, y);

    // mouse left click actions
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (state == GLFW_PRESS) {
        if (backButton->isMouseColliding(pos)) {
            engine->changeState(IntroState::instance());
        } else if (switchLight->isMouseColliding(pos)) {
            modelSelector = (modelSelector + 1) % lightModels.size();
            switchLight->setText(lightModels[modelSelector].second);
            switchLight->refreshLabelCenter();
        }
    }
}

void OptionState::update(GameEngine *engine) {}

void OptionState::draw(GameEngine *engine) {
    backButton->draw();
    switchLight->draw();
}

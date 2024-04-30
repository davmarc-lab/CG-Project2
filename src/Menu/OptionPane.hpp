#pragma once

#include "ButtonComponent.hpp"
#include "Component.hpp"

enum LightModel { PHONG, BLINN_PHONG, STANDARD };

class OptionPane {
private:
    vector<pair<LightModel, string>> lightModels = {
        pair<LightModel, string>(LightModel::PHONG, "Phong"),
        pair<LightModel, string>(LightModel::BLINN_PHONG, "Blinn-Phong"),
        pair<LightModel, string>(LightModel::STANDARD, "Standard"),
    };
    unsigned int modelSelector = 2;

    vector<Component *> buttons;

    ButtonComponent *backButton = new ButtonComponent(
        vec3(55, HEIGHT - 35, 0), color::WHITE, "Back", 40, color::BLACK);
    ButtonComponent *switchLight = new ButtonComponent(
        vec3(WIDTH / 2, HEIGHT / 2 + 200, 0), color::WHITE,
        this->lightModels[this->modelSelector].second, 30, color::BLACK);

public:
    OptionPane() {
        this->backButton->setScale(vec3(50, 30, 0));
        this->backButton->createVertexArray();
        this->switchLight->setScale(vec3(150, 20, 0));
        this->switchLight->createVertexArray();

        this->buttons.push_back(this->backButton);
        this->buttons.push_back(this->switchLight);
    }

    inline vector<Component *> getElements() { return this->buttons; }

    inline void switchModel() {
        this->modelSelector = (this->modelSelector + 1) % lightModels.size();
        this->switchLight->setText(lightModels[this->modelSelector].second);
    }

    inline bool execOperation(vec2 pos) {
        if (this->backButton->isMouseColliding(pos)) {
            this->clear();
            return true;
        } else if (this->switchLight->isMouseColliding(pos)) {
            this->switchModel();
            this->switchLight->refreshLabelCenter();
        }
        return false;
    }

    inline void clear() {
        for (const auto elem : this->buttons) {
            elem->clear();
        }
        this->buttons.clear();
    }

    ~OptionPane() = default;
};

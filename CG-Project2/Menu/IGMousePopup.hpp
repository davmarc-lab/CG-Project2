#pragma once

#include "IGMenu.hpp"

#include "../Action/ActionManager.hpp"

ActionManager *am = ActionManager::instance();

class IGMousePopup : public IGMenu {
private:
    const char *string_id;

    const char *actions[2] = {"Add", "Remove"};

    int action_selected = -1;

public:
    IGMousePopup(const char *id) : string_id(id) {}

    inline const char *getPopupStringId() { return this->string_id; }

    inline virtual void render() override {
        if (ImGui::BeginPopup(this->string_id)) {
            ImGui::SeparatorText("Utility Menu");

            for (int i = 0; i < IM_ARRAYSIZE(actions); i++)
                if (ImGui::Selectable(actions[i])) {
                    action_selected = i;
                    switch (i) {
                        case 0:
                            am->addAction(Action::ADD_ENTITY);
                            break;
                        case 1:
                            am->addAction(Action::DEL_ENTITY);
                        break;
                        default:
                            break;
                    }
                }

            ImGui::EndPopup();
        }
        // std::cout << action_selected << std::endl;
    }

    ~IGMousePopup() = default;
};

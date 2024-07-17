#pragma once

#include "IGMenu.hpp"

#include "../Action/ActionManager.hpp"

inline ActionManager *am = ActionManager::instance();

class IGMousePopup : public IGMenu {
  private:
    const char *string_id;

    const char *actions[2] = {"Add", "Remove"};
    const char *add_actions[3] = {"Cube", "Sphere", "Object"};

  public:
    IGMousePopup(const char *id) : string_id(id) {}

    inline const char *getPopupStringId() { return this->string_id; }

    inline virtual void render() override {
        if (ImGui::BeginPopup(this->string_id)) {
            ImGui::SeparatorText("Utility Menu");

            for (int i = 0; i < IM_ARRAYSIZE(actions); i++) {
                if (i == 0) {
                    if (ImGui::BeginMenu(actions[0])) {
                        for (int i = 0; i < IM_ARRAYSIZE(add_actions); i++) {
                            if (ImGui::Selectable(add_actions[i])) {
                                switch (i) {
                                    case 0:
                                        am->addAction(Action::ADD_CUBE_ENTITY);
                                        break;
                                    case 1:
                                        am->addAction(Action::ADD_SPHERE_ENTITY);
                                        break;
                                    case 2:
                                        am->addAction(Action::ADD_OBJECT_ENTITY);
                                    default:
                                        break;
                                }
                            }
                        }
                        ImGui::EndMenu();
                    }
                } else {
                    if (ImGui::Selectable(actions[i])) {
                        switch (i) {
                        case 1:
                            am->addAction(Action::DEL_ENTITY);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            ImGui::EndPopup();
        }
        // std::cout << action_selected << std::endl;
    }

    ~IGMousePopup() = default;
};

#pragma once

#include "IGMenu.hpp"

#include "../Action/ActionManager.hpp"

inline ActionManager *am = ActionManager::instance();

/*
 * This class creates a popup menu to 
 */
class IGMousePopup : public IGMenu {
  private:
    const char *string_id;

    bool open = false;

    const char *actions[1] = {"Add"};
    const char *add_actions[4] = {"Cube", "Sphere", "Object", "Light"};
    const char *light_types[3] = {"Directional", "Point", "Spot"};

  public:
    IGMousePopup(const char *id) : string_id(id) {}

    inline const char *getPopupStringId() { return this->string_id; }

    inline void openPopup() {
        this->open = true;
    }

    inline virtual void render() override {
        if (this->open)
            ImGui::OpenPopup(this->string_id);

        this->open = false;

        if (ImGui::BeginPopup(this->string_id)) {
            ImGui::SeparatorText("Utility Menu");

            // Print all available actions
            for (int i = 0; i < IM_ARRAYSIZE(actions); i++) {
                // If is the Add option draw this sub menu
                if (i == 0) {
                    if (ImGui::BeginMenu(actions[0])) {
                        // Print all Add actions
                        for (int i = 0; i < IM_ARRAYSIZE(add_actions); i++) {
                            // If is Lights, creates another submenu
                            if (i == 3) {
                                if (ImGui::BeginMenu(add_actions[3])) {
                                    // Print all light casters
                                    for (int j = 0; j < IM_ARRAYSIZE(light_types); j++) {
                                        // Creates the menu entry and when clicked execute these actions.
                                        if (ImGui::MenuItem(light_types[j])) {
                                            switch (j) {
                                            case 0:
                                                am->addAction(Action::ADD_DIRECT_LIGHT);
                                                break;
                                            case 1:
                                                am->addAction(Action::ADD_POINT_LIGHT);
                                                break;
                                            case 2:
                                                am->addAction(Action::ADD_SPOT_LIGHT);
                                                break;
                                            default:
                                                break;
                                            }
                                        }
                                    }
                                    ImGui::EndMenu();
                                }
                                continue;
                            }
                            // Creates a menu entry for each element in the array above
                            if (ImGui::MenuItem(add_actions[i])) {
                                switch (i) {
                                case 0:
                                    am->addAction(Action::ADD_CUBE_ENTITY);
                                    break;
                                case 1:
                                    am->addAction(Action::ADD_SPHERE_ENTITY);
                                    break;
                                case 2:
                                    am->addAction(Action::ADD_OBJECT_ENTITY);
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                        ImGui::EndMenu();
                    }
                }
            }

            ImGui::EndPopup();
        }
    }

    ~IGMousePopup() = default;
};

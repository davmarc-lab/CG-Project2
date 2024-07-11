#pragma once

#include "../Entity/Entity.hpp"

#include "IGMenu.hpp"

#include <vector>
#ifdef _WIN32
#include <string>
#include <sstream>
#endif

class IGEntity : public IGMenu {

private:
    Entity* obj_observer = nullptr;
    Shader* shader_observer = nullptr;

    enum AXIS {
        X, Y, Z, NONE
    };

    inline vec3 updateOtherValues(vec3 vec, AXIS axis) {
        float val = 0.f;

        switch (axis) {
            case AXIS::X: {
                val = vec.x;
                break;
            }
            case AXIS::Y: {
                val = vec.y;
                break;
            }
            case AXIS::Z: {
                val = vec.z;
                break;
            }
            default: {
                return vec3(vec);
            }
        }

        return vec = vec3(val);
    }

    inline AXIS findAxis(vector<AXIS> scaleAxis) {
        for (const auto elem : scaleAxis) {
            if (elem != AXIS::NONE)
            return elem;
        }
        return AXIS::NONE;
    }

public:
    IGEntity() {}

    IGEntity(Entity* entity, Shader* shader) : obj_observer(entity), shader_observer(shader) {}

    inline void changeObserver(Entity* e, Shader* s) {
        this->obj_observer = e;
        this->shader_observer = s;
        cout << this->shader_observer->getId() << endl;
    }

    inline virtual void render() override {
        ImGui::Begin("Entity");

        if (this->obj_observer == nullptr && this->shader_observer == nullptr) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "No Object Selected!");
        } else if (this->obj_observer == nullptr || this->shader_observer == nullptr) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Problems while selecting");
        } else {
            auto pos = this->obj_observer->getPosition();
            auto scale = this->obj_observer->getScale();
            auto rot = this->obj_observer->getRotation();

            static bool lockFlag = false;
            vector<AXIS> scaleAxis;

            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::Text("Position:");
                if (ImGui::DragFloat("x", &pos.x, 0.005, 0.f)) {
                    this->obj_observer->setMotionTime(0);
                }
                if (ImGui::DragFloat("y", &pos.y, 0.005)) {
                    this->obj_observer->setMotionTime(0);
                }
                if (ImGui::DragFloat("z", &pos.z, 0.005)) {
                    this->obj_observer->setMotionTime(0);
                }
                if (ImGui::Button("Reset")) {
                    pos = vec3(0);
                }
                this->obj_observer->setPosition(vec3(pos));

                ImGui::Text("Scale:");
                AXIS click;

                click = ImGui::DragFloat("x##2", &scale.x, 0.005) ? AXIS::X : AXIS::NONE;
                scaleAxis.push_back(click);
                click = ImGui::DragFloat("y##2", &scale.y, 0.005) ? AXIS::Y : AXIS::NONE;
                scaleAxis.push_back(click);
                click = ImGui::DragFloat("z##2", &scale.z, 0.005) ? AXIS::Z : AXIS::NONE;
                scaleAxis.push_back(click);

                if (ImGui::Button("Reset##2")) {
                    scale = vec3(1);
                }

                ImGui::Checkbox("Lock All", &lockFlag);
                if (lockFlag) {
                    scale = updateOtherValues(scale, findAxis(scaleAxis));
                }

                this->obj_observer->setScale(vec3(scale));

                ImGui::Text("Rotation:");
                ImGui::DragFloat("x##3", &rot.x, 0.005);
                ImGui::DragFloat("y##3", &rot.y, 0.005);
                ImGui::DragFloat("z##3", &rot.z, 0.005);
                if (ImGui::Button("Reset##3")) {
                    rot = vec3(0);
                }
                this->obj_observer->setRotation(vec3(rot));
            }

            if (ImGui::CollapsingHeader("Shader")) {
                // bad copy of the shader
                cout << this->shader_observer->getId() << endl;
                ImGui::Text("%d", this->shader_observer->getId());
            }

            if (ImGui::CollapsingHeader("Texture")) {
                ImGui::Text("Texture File:");
                ImGui::Text("%s", this->obj_observer->getTexture().getPath());
                if (ImGui::Button("Choose Texture")) {
                    IGFD::FileDialogConfig config;
                    config.path = "./resources/textures/";
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
                }
                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                    if (ImGuiFileDialog::Instance()->IsOk()) {
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

                        // action
#ifdef _WIN32
                        {
                            // make custom path for windows
                            std::vector<std::string> words;
                            std::string word;
                            std::istringstream stream(filePathName);
                            bool start_writing = false, add_sep = true;
                            string res = "./";

                            while (std::getline(stream, word, '\\')) {
                                std::istringstream subStream(word);
                                if (!start_writing && word == "resources") {
                                    start_writing = true;
                                }

                                if (start_writing) {
                                    res.append(word);
                                    res.append("/");
                                }
                            }

                            if (!res.empty()) {
                                res.pop_back();
                            }
                            filePathName = string(res);
                        }
#endif
                        this->obj_observer->changeTexture(filePathName);
                    }

                    // close
                    ImGuiFileDialog::Instance()->Close();
                }
            }
            bool aff_lights = this->obj_observer->isAffectedByLight();
            if (ImGui::Checkbox("Affected By Lights", &aff_lights)) {
                this->obj_observer->setLightComputation(aff_lights);
            }
        }

        ImGui::End();
    }

    ~IGEntity() = default;
};

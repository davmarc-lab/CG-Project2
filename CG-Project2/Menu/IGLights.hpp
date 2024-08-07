#pragma once

#include "IGMenu.hpp"

#include "../Light/DirectionalLight.hpp"
#include "../Light/Light.hpp"
#include "../Light/PointLight.hpp"
#include "../Light/SpotLight.hpp"

#include "../Action/ActionManager.hpp"

extern int max_lights;

/*
 * This class creates an ImGui panel to manage all the lights in the scene.
 */
class IGLights : public IGMenu {
  private:
    // List of all lights.
    vector<Light *> lights;

    // Selected light to be deleted.
    Light *del_light = nullptr;

    // Renders extra properties for directional light
    inline void renderDirectional(DirectionalLight *l) {
        ImGui::SeparatorText("Directional Light##1");
        auto dir = l->getDirection();
        if (ImGui::DragFloat3("Direction", &dir.x, 0.005f)) {
            l->setDirection(dir);
        }
    }

    // Renders extra properties for point lights
    inline void renderPoint(PointLight *l) {
        ImGui::SeparatorText("Point Light");

        auto con = l->getConstant();
        if (ImGui::DragFloat("Constant", &con, 0.005f)) {
            if (con > 0) {
                l->setConstant(con);
            }
        }

        auto lin = l->getLinear();
        if (ImGui::DragFloat("Linear", &lin, 0.0001f)) {
            if (lin > 0) {
                l->setLinear(lin);
            }
        }

        auto quad = l->getQuadratic();
        if (ImGui::DragFloat("Quadratic", &quad, 0.0001f)) {
            if (quad > 0) {
                l->setQuadratic(quad);
            }
        }
    }

    // Renders extra properties for spot lights
    inline void renderSpot(SpotLight *l) {
        ImGui::SeparatorText("Spot Light");

        auto dir = l->getDirection();
        if (ImGui::DragFloat3("Direction", &dir.x, 0.005f)) {
            l->setDirection(dir);
        }
        auto con = l->getConstant();
        if (ImGui::DragFloat("Constant", &con, 0.0005f)) {
            if (con > 0) {
                l->setConstant(con);
            }
        }

        auto lin = l->getLinear();
        if (ImGui::DragFloat("Linear", &lin, 0.0001f)) {
            if (lin > 0) {
                l->setLinear(lin);
            }
        }

        auto quad = l->getQuadratic();
        if (ImGui::DragFloat("Quadratic", &quad, 0.0001f)) {
            if (quad > 0) {
                l->setQuadratic(quad);
            }
        }

        auto coff = l->getCutOff();
        if (ImGui::DragFloat("CutOff", &coff, 0.05f)) {
            if (coff > 0) {
                l->setCutOff(coff);
            }
        }

        auto out = l->getOuterCutOff();
        if (ImGui::DragFloat("Outer CutOff", &out, 0.05f)) {
            if (out > 0) {
                l->setOuterCutOff(out);
            }
        }
    }

  public:
    IGLights() {}

    IGLights(vector<Light *> lights) : lights(lights) {}

    inline void refreshLights(vector<Light *> lights) { this->lights = lights; }

    inline Light *getLightToDelete() { return this->del_light; }

    inline virtual void render() override {
        ImGui::Begin("Lights");

        ImGui::SeparatorText("Settings");

        ImGui::InputInt("##", &max_lights, 1, 5);
        ImGui::SameLine();
        ImGui::TextWrapped("Max Lights in Scene");
        if (max_lights < 0) {
            max_lights = 0;
        }

        ImGui::SeparatorText("Light Sources");
        // Print all lights common properties and after it render the extra ones.
        for (auto l : this->lights) {
            ImGui::PushID(l);
            auto type = getLightTypeName(l->getType());
            if (ImGui::CollapsingHeader(string(type + " Light").c_str())) {
                ImGui::SeparatorText("Light Info");

                auto col = l->getColor();
                if (ImGui::ColorEdit3("Color", &col.x)) {
                    l->setColor(col);
                }

                auto intens = l->getIntensity();
                if (ImGui::DragFloat("Intensity", &intens, 0.05f, 0.0f)) {
                    if (intens > 0) {
                        l->setIntensity(intens);
                    }
                }

                ImGui::SeparatorText("Light Parameters");
                auto amb = l->getAmbient();
                if (ImGui::DragFloat3("Ambient", &amb.x, 0.005f, 0.0f, 1.0f)) {
                    l->setAmbient(amb);
                }

                auto diff = l->getDiffuse();
                if (ImGui::DragFloat3("Diffuse", &diff.x, 0.005f, 0.0f, 1.0f)) {
                    l->setDiffuse(diff);
                }

                auto spec = l->getSpecular();
                if (ImGui::DragFloat3("Specular", &spec.x, 0.005f, 0.0f, 1.0f)) {
                    l->setSpecular(spec);
                }

                switch (l->getType()) {
                case LightType::DIRECTIONAL:
                    renderDirectional((DirectionalLight *)l);
                    break;
                case LightType::POINTLIGHT:
                    renderPoint((PointLight *)l);
                    break;
                case LightType::SPOTLIGHT:
                    renderSpot((SpotLight *)l);
                    break;
                default:
                    break;
                }
            }

            // If clicked adds an Action that will be handled in the PlayState
            if (ImGui::Button("Delete")) {
                this->del_light = l;
                ActionManager::instance()->addAction(Action::DEL_CUSTOM_LIGHT);
            }

            ImGui::Separator();

            ImGui::PopID();
        }
        ImGui::End();
    }

    ~IGLights() = default;
};

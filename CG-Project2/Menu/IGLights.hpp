#pragma once

#include "IGMenu.hpp"

#include "../Light/Light.hpp"
#include "../Light/DirectionalLight.hpp"
#include "../Light/PointLight.hpp"
#include "../Light/SpotLight.hpp"

extern int max_lights;

class IGLights : public IGMenu {
private:
    vector<Light *> lights;

    inline void renderDirectional(DirectionalLight *l) {
        auto dir = l->getDirection();
        if (ImGui::DragFloat3("Direction", &dir.x, 0.005f)) {
            l->setDirection(dir);
        }
    }

    inline void renderPoint(PointLight *l) {
        auto pos = l->getPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.005f)) {
            l->setPosition(pos);
        }

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

    inline void renderSpot(SpotLight *l) {
        auto pos = l->getPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.005f)) {
            l->setPosition(pos);
        }

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

    inline virtual void render() override {
        ImGui::Begin("Lights");

        ImGui::TextWrapped("Settings");

        ImGui::InputInt("Max Lights in Scene", &max_lights, 1, 5);
        if (max_lights < 0) {
            max_lights = 0;
        }

        for (auto l : this->lights) {
            ImGui::PushID(l);
            auto type = getLightTypeName(l->getType());
            if (ImGui::CollapsingHeader(string(type + " Light").c_str())) {
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

            ImGui::PopID();
        }
        ImGui::End();
    }

    ~IGLights() = default;
};

#include "Scene.hpp"
#include <algorithm>

void Scene::addElement(Entity *e, Shader *s) {
    if (!e->isInstanced()) {
        e->createVertexArray();
    }
    this->elements.push_back(std::make_pair(e, s));
}

void Scene::addCustomObj(Object *o, Shader *s) { this->custom_obj.push_back(std::make_pair(o, s)); }

bool Scene::removeElement(Entity *e, Shader *s, bool is_light_selected = false, Light *light_selcted = nullptr) {
    // if is selected a light, delete from light scene
    if (is_light_selected) {
        return this->lights.erase(std::find(this->lights.begin(), this->lights.end(), light_selcted)) != this->lights.end();
    }
    return this->elements.erase(std::remove(this->elements.begin(), this->elements.end(), pair(e, s)), this->elements.end()) != this->elements.end();
}

void Scene::addLight(Light *l) {
    if (!l->isCasterInstaced())
        l->initCaster();
    this->lights.push_back(l);
}

void Scene::draw() {
    int index = 0;

    for (auto e : this->elements) {
        // if the entity is affected by lights, checks if there are lights
        if (e.first->isAffectedByLight()) {
            e.second->use();
            // send data to shader to compute lights or not
            e.second->setInt("num_lights", this->lights.size());
            e.second->setInt("light_comp", e.first->getLightComputation());

            // If there is a light in the scene sends data
            if (this->lights.size() >= 1) {
                index = 0;
                for (auto l : this->lights) {
                    l->sendDataToShader(*e.second, index);
                    index++;
                }
            }
        }
        e.first->draw(*e.second);
    }

    if (this->lights.size() >= 1) {
        // draw light caster
        for (auto l : this->getLights()) {
            if (l->showCaster()) {
                l->drawCaster();
            }
        }
    }
}

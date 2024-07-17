#include "Scene.hpp"

void Scene::addElement(Entity *e, Shader *s) {
    if (!e->isInstanced()) {
        e->createVertexArray();
    }
    this->elements.push_back(std::make_pair(e, s));
}

void Scene::addCustomObj(Object *o, Shader *s) { this->custom_obj.push_back(std::make_pair(o, s)); }

bool Scene::removeElement(Entity *e, Shader *s) {
    this->elements.erase(std::remove(this->elements.begin(), this->elements.end(), pair(e, s)), this->elements.end());
    return false;
}

void Scene::addLight(Light *l) {
    this->lights.push_back(l);
}

void Scene::draw() {

    int index = 0;

    for (auto e : this->elements) {
        // If there is a light in the scene check if the entity is affected by lights
        if (this->lights.size() >= 1) {
            // send data to shader to compute lights or not
            e.second->use();
            e.second->setInt("light_comp", e.first->getLightComputation());

            // if the entity is affected by lights, send lights data
            if (e.first->isAffectedByLight()) {
                e.second->setInt("num_lights", this->lights.size());
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

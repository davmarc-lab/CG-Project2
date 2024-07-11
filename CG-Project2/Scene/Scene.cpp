#include "Scene.hpp"

void Scene::addElement(Entity *e, Shader s) { this->elements.push_back(std::make_pair(e, s)); }

bool Scene::removeElement(Entity *e) {
    warning("Remove Element NOT Implemented");

    return false;
}

void Scene::addLight(Light *l) {
    if (this->lights.size() > 1) {
        warning("Multiple lights still doesn't work");
        // return;
    }

    this->lights.push_back(l);
}

void Scene::draw() {

    for (auto e : this->elements) {
        // If there is a light in the scene check if the entity is affected by lights
        if (this->lights.size() >= 1) {
            // send data to shader to compute lights or not
            e.second.use();
            e.second.setBool("affectedByLights", e.first->isAffectedByLight());
            
            // if the entity is affected by lights, send lights data
            if (e.first->isAffectedByLight()) {
                this->lights[0]->sendDataToShader(e.second);
            }
        }


        e.first->draw(e.second);
    }
}

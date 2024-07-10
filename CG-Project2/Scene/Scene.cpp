#include "Scene.hpp"

void Scene::addElement(Entity *e, Shader s) { this->elements.push_back(std::make_pair(e, s)); }

bool Scene::removeElement(Entity *e) {
    warning("Remove Element NOT Implemented");

    return false;
}

void Scene::addLight(Light *l) {
    if (this->lights.size() > 1) {
        warning("Cannot work with multiple lights");
        return;
    }

    this->lights.push_back(l);
}

void Scene::draw() {
    for (auto e : this->elements) {
        // only 1 light
        if (this->lights.size() >= 1) {
            this->lights[0]->sendDataToShader(e.second);
        }
        e.first->draw(e.second);
    }
}

#include "Scene.hpp"

void Scene::addElement(Entity* e, Shader s) {
	this->elements.push_back(std::make_pair(e, s));
}

bool Scene::removeElement(Entity* e) {
	warning("Remove Element NOT Implemented");

	return false;
}

void Scene::draw() {
	for (auto e : this->elements) {
		e.first->draw(e.second);
	}
}
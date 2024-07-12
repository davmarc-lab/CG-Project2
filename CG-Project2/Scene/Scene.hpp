#pragma once

#include "../Entity/Entity.hpp"
#include "../Shader/Shader.hpp"

#include "../Light/Light.hpp"

#include <cassert>
#include <vector>

class Scene {
private:

	vector<std::pair<Entity*, Shader*>> elements;

    vector<Light*> lights;

public:
	Scene() {}

	void addElement(Entity* e, Shader* shader);

	bool removeElement(Entity* e);

	inline vector<std::pair<Entity*, Shader*>> getElements() { return this->elements; }

    void addLight(Light* l);

    inline vector<Light*> getLights() { return this->lights; }

	void draw();

	inline void clear() {
		for (auto elem : this->elements) {
			// elem.first->clear();
			elem.second->clear();
		}

		this->elements.clear();
        this->lights.clear();
		delete this;
	}

	~Scene() = default;
};

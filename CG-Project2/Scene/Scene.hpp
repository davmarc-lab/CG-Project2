#pragma once

#include "../Entity/Entity.hpp"
#include "../Shader/Shader.hpp"

#include <vector>

class Scene {
private:
	
	vector<std::pair<Entity*, Shader>> elements;

public:
	Scene() {}

	void addElement(Entity* e, Shader shader);

	bool removeElement(Entity* e);

	inline vector<std::pair<Entity*, Shader>> getElements() { return this->elements; }

	void draw();

	inline void clear() {
		for (auto elem : this->elements) {
			// elem.first->clear();
			elem.second.clear();
		}

		this->elements.clear();
		delete this;
	}

	~Scene() = default;
};
#pragma once

#include <map>

#include <vector>

#include "../../../Opengl-Core/include/Core.hpp"

class BasicScene {
public:
	void init(const Shared<ogl::Camera> &mainCamera) { this->mainCamera = mainCamera; }

	Shared<ogl::Camera> getCamera() const { return this->mainCamera; }

	void addEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);
	void removeEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);
	void removeEntity(const unsigned int &ett);

	std::map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> getShaderEntityMap() const { return this->m_entities; }

	BasicScene(BasicScene &other) = delete;

	void operator=(const BasicScene &other) = delete;

	/*
	 * Retrieves the instance of the BasicScene if it's not created.
	 * This function is thread safe using a simple `std::mutex`.
	 *
	 * @return `BasicScene` unique object.
	 */
	inline static Shared<BasicScene> instance() {
		if (s_pointer == nullptr) {
			Shared<BasicScene> copy(new BasicScene());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	~BasicScene() = default;

private:
	std::map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> m_entities{};

	Shared<ogl::Camera> mainCamera{};

	inline static Shared<BasicScene> s_pointer = nullptr;

	BasicScene() = default;
};

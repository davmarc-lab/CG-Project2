#pragma once

#include <vector>
#include "../../../Opengl-Core/include/Core.hpp"
#include "PBMaterial.hpp"

class PBScene {
public:
	void addEntity(const unsigned int &id) { this->m_entities.push_back(id); }

	std::vector<unsigned int> getEntities() const { return this->m_entities; }

	PBScene(PBScene &other) = delete;

	void operator=(const PBScene &other) = delete;

	/**
	 * @brief Retrieves the instance of the PBScene.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<PBScene> object
	 */
	inline static Shared<PBScene> instance() {
		if (s_pointer == nullptr) {
			Shared<PBScene> copy(new PBScene());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

private:
	PBScene() = default;

	inline static Shared<PBScene> s_pointer = nullptr;

	std::vector<unsigned int> m_entities{};
};

namespace systems {
	namespace pbr {
		void updateMaterial(const unsigned int &id, const PBMaterial &material);

		Shared<PBMaterial> getMaterial(const unsigned int &id);
	} // namespace pbr
	namespace render {
		void renderScene(const Shared<ogl::ShaderProgram> &shader, ogl::WorldCamera& world);
	}
} // namespace systems

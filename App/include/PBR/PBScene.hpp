#pragma once

#include <vector>
#include "../../../Opengl-Core/include/Core.hpp"
#include "PBMaterial.hpp"

/**
 * @brief This class manages a scene using meshes with PBRMaterial.
 */
class PBScene {
public:
	/**
	 * @brief Adds an entity to the scene.
	 *
	 * @param id the entity id
	 */
	void addEntity(const unsigned int &id) { this->m_entities.push_back(id); }

	/**
	 * @brief Retrieves all the entities id in the scene.
	 *
	 * @return a vector with all the entities
	 */
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

	/// static shared pointer for Singleton
	inline static Shared<PBScene> s_pointer = nullptr;

	/// entities in the scene
	std::vector<unsigned int> m_entities{};
};

namespace systems {
	/**
	 * @namespace pbr
	 * @brief Contains all the systems for entities with pbr materials.
	 */
	namespace pbr {

		/**
		 * @brief Updates the matiral of the given entity.
		 *
		 * @param id the entity id
		 * @param material the entity new material
		 */
		void updateMaterial(const unsigned int &id, const PBMaterial &material);

		/**
		 * @brief Retrives the PBRMaterial of the given entity.
		 *
		 * @param id the entity id
		 *
		 * @return a PBRMaterial of an entity
		 */
		Shared<PBMaterial> getMaterial(const unsigned int &id);
	} // namespace pbr

	namespace render {
		/**
		 * @brief This system renders a scene using PBRMaterial meshes with the given ShaderProgram.
		 *
		 * @param shader the PBR shader program
		 * @param world the world camera
		 */
		void renderScene(const Shared<ogl::ShaderProgram> &shader, ogl::WorldCamera &world);
	} // namespace render
} // namespace systems

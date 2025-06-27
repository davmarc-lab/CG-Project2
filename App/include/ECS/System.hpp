#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"

/**
 * @brief Data structure for instanced mesh rendering.
 * It stores for each primitive its color and model matrix.
 */
struct InstanceData {
	/// spheres counter
	unsigned int nspheres = 0;
	/// spheres id
	std::vector<unsigned int> spheresIds{};
	/// spheres color
	std::vector<glm::vec4> spheresColor{};
	/// spheres model matrix
	std::vector<glm::mat4> spheresModel{};
} inline data;

/**
 * @namespace systems
 *
 * @brief It contains all the ECS systems.
 */
namespace systems {

	/**
	 * @namespace systems::ecs
	 *
	 * @brief Utility methods to manage the entities in the EntityManager.
	 * @see EntityManager
	 */
	namespace ecs {
		/**
		 * @brief Removes an entity from the EntityManager, if the entity has children
		 * they will be removed too.
		 *
		 * @param id entity id
		 * @return true if the entity is removed
		 */
		bool removeEntityFromManager(const unsigned int &id);

		/**
		 * @brief Removes the given entity from the BasicScene, if entity has children
		 * they will be removed too.
		 *
		 * @param id entity id
		 */
		void removeEntityFromScene(const unsigned int &id);

		/**
		 * @brief Removes an entity from the EntityManager and from the BasicScene.
		 *
		 * @param id entity id
		 * @return true if the entity is removed
		 */
		bool removeEntityFromAll(const unsigned int &id);

		/**
		 * @brief Clean the EntityManager from all entities and Components and from
		 * the BasicScene too.
		 */
		void cleanAll();

		/**
		 * @brief Retrieves the name of the given entity.
		 *
		 * @param id entity id
		 * @return the name of the entity
		 */
		std::string getEntityName(const unsigned int &id);

		/**
		 * @brief Updates the name of the given entity.
		 *
		 * @param id entity id
		 * @param name entity new name
		 */
		void updateEntityName(const unsigned int &id, const std::string &name);
	} // namespace ecs

	/**
	 * @namespace systems::transform
	 * @brief This namespace contains all the methods to manipulate the Transform component.
	 *
	 * @note It calls the Transform component methods already defined.
	 * @see Transform
	 */
	namespace transform {
		/**
		 * @brief Updates the position of the given entity.
		 *
		 * @param id entity id
		 * @param position entity new position
		 */
		void updatePosition(const unsigned int &id, const glm::vec3 &position);

		/**
		 * @brief Updates the scale of the given entity.
		 *
		 * @param id entity id
		 * @param scale entity new scale
		 */
		void updateScale(const unsigned int &id, const glm::vec3 &scale);

		/**
		 * @brief Updates the rotation of the given entity.
		 *
		 * It takes an Euler angle `glm::vec3` in degrees and it will be converted in
		 * radians by the component.
		 *
		 * @param id entity id
		 * @param rotation entity new rotation in degrees
		 */
		void updateRotation(const unsigned int &id, const glm::vec3 &rotation);

		/**
		 * @brief Adds an offset to the position of the given entity
		 *
		 * @param id entity id
		 * @param offset position offset to be added
		 */
		void addPosition(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Adds an offset to the scale of the given entity
		 *
		 * @param id entity id
		 * @param offset scale offset to be added
		 */
		void addScale(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Adds an offset to the rotation of the given entity
		 *
		 * It works the same way of `updateRotation`.
		 * @see systems::transform::updateRotation()
		 *
		 * @param id entity id
		 * @param offset rotation offset to be added
		 */
		void addRotation(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Retrieves the position of the given entity.
		 *
		 * @param id entity id
		 * @return the entity position
		 */
		glm::vec3 getPosition(const unsigned int &id);

		/**
		 * @brief Retrieves the scale of the given entity.
		 *
		 * @param id entity id
		 * @return the entity scale
		 */

		glm::vec3 getScale(const unsigned int &id);

		/**
		 * @brief Retrieves the rotation of the given entity.
		 *
		 * @param id entity id
		 * @return the entity rotation in Euler angle.
		 */
		glm::vec3 getRotation(const unsigned int &id);

		/**
		 * @brief Retrieves the model matrix of the given entity.
		 *
		 * If the model matrix is dirty it will be updated by the component automatically.
		 *
		 * @param id entity id
		 * @return the entity model matrix
		 */
		glm::mat4 getModelMatrix(const unsigned int &id);

		/**
		 * @brief Updates the model matrix of the given entity.
		 *
		 * If the model matrix is dirty it will update the model matrix and also the entity
		 * collider if it has one.
		 *
		 * @param id entity id
		 */
		void updateModelMatrix(const unsigned int &id);

		/**
		 * @brief Updates all model matrix of each entity with the Transform component.
		 */
		void updateAllModelMatrix();
	} // namespace transform

	/**
	 * @namespace systems::collision
	 *
	 * @brief It contains all utilities to retrieve, handle and update mesh ColliderComponent.
	 *
	 * @see ColliderComponent
	 */
	namespace collision {
		/**
		 * @brief
		 * @param id entity id
		 */
		void updateParentCollider(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateCollider(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateAllColliders();

		/**
		 * @brief
		 * @param id entity id
		 */
		ColliderType getColliderType(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateColliderType(const unsigned int &id, const ColliderType &type);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateSimulated(const unsigned int &id, const bool &simulated);

		/**
		 * @brief
		 * @param id entity id
		 */
		void compressBoundingBox();

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		float getEnemyLastHit(const unsigned int &id);
		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		float getPlayerLastHit(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		Pair<glm::vec3> getCollider(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateEnemyLastHit(const unsigned int &id, const float &time);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updatePlayerLastHit(const unsigned int &id, const float &time);

		/**
		 * @brief Retrieves all entities overlapping each other using simple
		 * AABB detection.
		 *
		 * @return a `std::vector<Pair<unsigned int>>` containing all overlapping entities
		 */
		std::vector<Pair<unsigned int>> getCollisions();
	} // namespace collision

	namespace camera {
		Shared<ogl::Camera> getCamera(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateCameraCollider(const unsigned int &id, const glm::vec3 &position, const glm::vec3 &size);
	} // namespace camera

	namespace parent {
		/**
		 * @brief
		 * @param id entity id
		 */
		void addChild(const unsigned int &parent, const unsigned int &child);

		/**
		 * @brief
		 * @param id entity id
		 */
		void removeChild(const unsigned int &parent, const unsigned int &child);
	} // namespace parent

	namespace input {
		/**
		 * @brief
		 * @param id entity id
		 */
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func);
	} // namespace input

	namespace animation {
		/**
		 * @brief
		 * @param id entity id
		 */
		void executeNextFrame(const float &currentTime);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateDistanceAnimation();
	} // namespace animation

	namespace texture {
		ogl::Texture getTexture(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void setTexture(const unsigned int &id, const ogl::Texture &texture);

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		std::string getTexturePath(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateTexturePath(const unsigned int &id, const std::string &path);

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		bool isMeshReflective(const unsigned int &id);
		/**
		 * @brief
		 * @param id entity id
		 */
		void setMeshReflective(const unsigned int &id, const bool &val);
	} // namespace texture

	namespace material {
		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		Material getMaterial(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		std::string getMaterialName(const unsigned int &id);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateAmbient(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateDiffuse(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateSpecular(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateShininess(const unsigned int &id, const float &val);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateMaterial(const unsigned int &id, const Material &material);
	} // namespace material

	namespace light {
		LightShaderBlock getLightInfo(const unsigned int &id);
		/**
		 * @brief
		 * @param id entity id
		 */
		void updateColor(const unsigned int &id, const glm::vec3 &color);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateIntensity(const unsigned int &id, const float &intensity);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateAmbient(const unsigned int &id, const glm::vec3 &ambient);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateDiffuse(const unsigned int &id, const glm::vec3 &diffuse);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateSpecular(const unsigned int &id, const glm::vec3 &specular);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateDirection(const unsigned int &id, const glm::vec3 &direction);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updatePosition(const unsigned int &id, const glm::vec3 &position);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateConstant(const unsigned int &id, const float &constant);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateLinear(const unsigned int &id, const float &linear);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateQuadratic(const unsigned int &id, const float &quadratic);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateCutoff(const unsigned int &id, const float &cutoff);

		/**
		 * @brief
		 * @param id entity id
		 */
		void updateOuterCutoff(const unsigned int &id, const float &outerCutoff);

		/**
		 * @brief
		 * @param id entity id
		 */
		void setLightComputation(const unsigned int &id, const LightComputation &comp);

		/**
		 * @brief
		 * @param id entity id
		 * @return
		 */
		LightComputation getLightComputation(const unsigned int &id);
	} // namespace light

	namespace render {
		/**
		 * @brief
		 * @param id entity id
		 */
		void initStencilShader();

		/**
		 * @brief
		 * @param id entity id
		 */
		void prepareInstancedMesh(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors);

		/**
		 * @brief
		 * @param id entity id
		 */
		void renderSkybox(const unsigned int &id, const Shared<ogl::ShaderProgram> &shader);

		/**
		 * @brief
		 */
		void renderAllMeshes();

		/**
		 * @brief
		 */
		void renderInstancedMeshes();

		/**
		 * @brief
		 */
		void renderBoundingBox();
	} // namespace render
} // namespace systems

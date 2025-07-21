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
	 * @brief All utilities to manage the entities in the EntityManager.
	 * @see EntityManager
	 */
	namespace ecs {
		/**
		 * @brief Removes an entity from the EntityManager, if the entity has children
		 * they will be removed too.
		 *
		 * @param id the entity id
		 * @return true if the entity is removed
		 */
		bool removeEntityFromManager(const unsigned int &id);

		/**
		 * @brief Removes the given entity from the BasicScene, if entity has children
		 * they will be removed too.
		 *
		 * @param id the entity id
		 */
		void removeEntityFromScene(const unsigned int &id);

		/**
		 * @brief Removes an entity from the EntityManager and from the BasicScene.
		 *
		 * @param id the entity id
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
		 * @param id the entity id
		 * @return the name of the entity
		 */
		std::string getEntityName(const unsigned int &id);

		/**
		 * @brief Updates the name of the given entity.
		 *
		 * @param id the entity id
		 * @param name the entity new name
		 */
		void updateEntityName(const unsigned int &id, const std::string &name);
	} // namespace ecs

	/**
	 * @namespace systems::transform
	 * @brief All the methods to manipulate the Transform component.
	 *
	 * @note It calls the Transform component methods already defined.
	 * @see Transform
	 */
	namespace transform {
		/**
		 * @brief Updates the position of the given entity.
		 *
		 * @param id the entity id
		 * @param position the entity new position
		 */
		void updatePosition(const unsigned int &id, const glm::vec3 &position);

		/**
		 * @brief Updates the scale of the given entity.
		 *
		 * @param id the entity id
		 * @param scale the entity new scale
		 */
		void updateScale(const unsigned int &id, const glm::vec3 &scale);

		/**
		 * @brief Updates the rotation of the given entity.
		 *
		 * It takes an Euler angle `glm::vec3` in degrees and it will be converted in
		 * radians by the component.
		 *
		 * @param id the entity id
		 * @param rotation the entity new rotation in degrees
		 */
		void updateRotation(const unsigned int &id, const glm::vec3 &rotation);

		/**
		 * @brief Adds an offset to the position of the given entity
		 *
		 * @param id the entity id
		 * @param offset the position offset to be added
		 */
		void addPosition(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Adds an offset to the scale of the given entity
		 *
		 * @param id the entity id
		 * @param offset the scale offset to be added
		 */
		void addScale(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Adds an offset to the rotation of the given entity
		 *
		 * It works the same way of `updateRotation`.
		 * @see systems::transform::updateRotation()
		 *
		 * @param id the entity id
		 * @param offset the rotation offset to be added
		 */
		void addRotation(const unsigned int &id, const glm::vec3 &offset);

		/**
		 * @brief Retrieves the position of the given entity.
		 *
		 * @param id the entity id
		 * @return the entity position
		 */
		glm::vec3 getPosition(const unsigned int &id);

		/**
		 * @brief Retrieves the scale of the given entity.
		 *
		 * @param id the entity id
		 * @return the entity scale
		 */

		glm::vec3 getScale(const unsigned int &id);

		/**
		 * @brief Retrieves the rotation of the given entity.
		 *
		 * @param id the entity id
		 * @return the entity rotation in Euler angle.
		 */
		glm::vec3 getRotation(const unsigned int &id);

		/**
		 * @brief Retrieves the model matrix of the given entity.
		 *
		 * If the model matrix is dirty it will be updated by the component automatically.
		 *
		 * @param id the entity id
		 * @return the entity model matrix
		 */
		glm::mat4 getModelMatrix(const unsigned int &id);

		/**
		 * @brief Updates the model matrix of the given entity.
		 *
		 * If the model matrix is dirty it will update the model matrix and also the entity
		 * collider if it has one.
		 *
		 * @param id the entity id
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
		 * @brief Updates the collider of a parent mesh.
		 *
		 * @note This method only updates the collider of the father entity and
		 * not his children.
		 *
		 * @param id the parent entity id
		 */
		void updateParentCollider(const unsigned int &id);

		/**
		 * @brief Updates the collider of the given entity.
		 * If this entity is a parent entity (an entity with a ParentComponent),
		 * it will call systems::collision::updateParentCollider() instead.
		 *
		 * In the other case it will normally update the collider using an
		 * Optimized Bounding Box.
		 *
		 * @see ColliderComponent
		 *
		 * @param id the entity id
		 */
		void updateCollider(const unsigned int &id);

		/**
		 * @brief Updates all the entities collider by calling systems::collision::updateCollider().
		 */
		void updateAllColliders();

		/**
		 * @brief Retrieves the type of the entity collider.
		 *
		 * @param id the entity id
		 * @return the collider type
		 */
		ColliderType getColliderType(const unsigned int &id);

		/**
		 * @brief Updates the collider type of the given entity.
		 *
		 * @param id the entity id
		 * @param type the collider type
		 */
		void updateColliderType(const unsigned int &id, const ColliderType &type);

		/**
		 * @brief Updates the simulation of the entity in a PhysicWorld.
		 *
		 * @note If an entity is not simulated means that it's a static entity.
		 *
		 * @param id the entity id
		 * @param simulated if true the entity is simulated
		 */
		void updateSimulated(const unsigned int &id, const bool &simulated);

		/**
		 * @brief This method optimize children bounding box of all parent entities in a
		 * single square/cube bounding box.
		 */
		void compressBoundingBox();

		/**
		 * @brief Retrieves the collider corners of the entity given.
		 *
		 * @param id the entity id
		 * @return a pair of (botLeft, topRight) corner
		 */
		Pair<glm::vec3> getCollider(const unsigned int &id);

		/**
		 * @brief Calculate if two entities are colliding or not.
		 *
		 * @param first the first entity id
		 * @param second the second entity id
		 * @return true if the two entities are colliding
		 */
		bool isColliding(const unsigned int &first, const unsigned int &second);

		/**
		 * @brief Retrieves all entities overlapping each other using simple
		 * AABB detection.
		 *
		 * @return a `std::vector<Pair<unsigned int>>` containing all overlapping entities
		 */
		std::vector<Pair<unsigned int>> getCollisions();
	} // namespace collision

	/**
	 * @namespace systems::camera
	 * @brief All utilities to manage the world camera.
	 */
	namespace camera {
		/**
		 * @brief Retrieves a pointer to the camera given.
		 *
		 * @param id the camera id
		 * @return a shared pointer to the camera
		 */
		Shared<ogl::Camera> getCamera(const unsigned int &id);

		/**
		 * @brief Updates the collider of the camera given.
		 *
		 * @param id the camera id
		 * @param position the camera position
		 * @param size the camera collider size
		 */
		void updateCameraCollider(const unsigned int &id, const glm::vec3 &position, const glm::vec3 &size);
	} // namespace camera

	/**
	 * @namespace systems::parent
	 * @brief All utilities for a parent entity.
	 */
	namespace parent {
		/**
		 * @brief Adds a child to the parent entity.
		 *
		 * @param parent the parent entity id
		 * @param child the child entity id
		 */
		void addChild(const unsigned int &parent, const unsigned int &child);

		/**
		 * @brief Removes the given child from the parent entity.
		 *
		 * @param parent the parent entity id
		 * @param child the child entity id
		 */
		void removeChild(const unsigned int &parent, const unsigned int &child);
	} // namespace parent

	/**
	 * @namespace systems::input
	 * @brief All utilities to manage user input to interact
	 * whith meshes.
	 */
	namespace input {
		/**
		 * @brief Retrieves all the callback of a mesh to be called when the key is pressed.
		 *
		 * @param id the entity id
		 * @return a vector of pair like (key, callback) where the key is an `unsigned int`
		 * and the callback is a `std::funcion<void()>`
		 */
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id);

		/**
		 * @brief Sets a callback for the entity.
		 *
		 * @param id the entity id
		 * @param key the key that execute the callback
		 * @param func the callback to be executed
		 */
		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func);
	} // namespace input

	/**
	 * @namespace systems::animation
	 * @brief All utilities to handle animations.
	 */
	namespace animation {
		/**
		 * @brief This method updates all the TimeAnimation components, it needs
		 * the curren time in seconds.
		 *
		 * The current time is used to stop dead animations.
		 * @see TimeAnimation
		 *
		 * @param currentTime the current time in seconds
		 */
		void executeNextFrame(const float &currentTime);
	} // namespace animation

	/**
	 * @namespace systems::texture
	 * @brief All utilities to manage entities texture.
	 * @see TextureComponent
	 * @see ogl::Texture
	 */
	namespace texture {
		/**
		 * @brief Retrieves the texture of the given entity.
		 *
		 * @param id the entity id
		 *
		 * @return an ogl::Texture object
		 */
		ogl::Texture getTexture(const unsigned int &id);

		/**
		 * @brief Sets the texture for the given entity.
		 *
		 * @param id the entity id
		 * @param texture the entity texture
		 */
		void setTexture(const unsigned int &id, const ogl::Texture &texture);

		/**
		 * @brief Retrieves the entity texture path.
		 *
		 * @param id the entity id
		 * @return a path to the texture file
		 */
		std::string getTexturePath(const unsigned int &id);

		/**
		 * @brief Updates the entity texture path.
		 *
		 * @param id the entity id
		 * @param path the new texture path
		 */
		void updateTexturePath(const unsigned int &id, const std::string &path);

		/**
		 * @brief This method sets the reflection of an entity.
		 * If the entity is reflective it will reflect the skybox texture.
		 *
		 * @param id the entity id
		 * @return true if the entity is reflective
		 */
		bool isMeshReflective(const unsigned int &id);

		/**
		 * @brief Sets the reflectiveness of an entity.
		 *
		 * @param id the entity id
		 * @param val if `true` the entity is reflective
		 */
		void setMeshReflective(const unsigned int &id, const bool &val);
	} // namespace texture

	/**
	 * @namespace systems::material
	 * @brief All utilities to manage entity material.
	 * @see MaterialComponent
	 * @see Material
	 */
	namespace material {
		/**
		 * @brief Retrieves the material of the given entity.
		 *
		 * @param id the entity id
		 * @return the material of the entity
		 */
		Material getMaterial(const unsigned int &id);

		/**
		 * @brief Retrieves the material name of the entity given.
		 *
		 * @param id the entity id
		 * @return the name of the material
		 */
		std::string getMaterialName(const unsigned int &id);

		/**
		 * @brief Updates the ambient vector of a material.
		 *
		 * @param id the entity id
		 * @param val the ambient value
		 */
		void updateAmbient(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief Updates the diffuse vector of a material.
		 *
		 * @param id the entity id
		 * @param val the diffuse value
		 */
		void updateDiffuse(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief Updates the specular vector of a material.
		 *
		 * @param id the entity id
		 * @param val the specular value
		 */
		void updateSpecular(const unsigned int &id, const glm::vec3 &val);

		/**
		 * @brief Updates the shininess value of a material.
		 *
		 * @param id the entity id
		 * @param val the shininess value
		 */
		void updateShininess(const unsigned int &id, const float &val);

		/**
		 * @brief Updates the material of the entity given.
		 *
		 * @param id the entity id
		 * @param material the new material
		 */
		void updateMaterial(const unsigned int &id, const Material &material);
	} // namespace material

	/**
	 * @namespace systems::light
	 * @brief All utilities for lights and shader data.
	 */
	namespace light {
		/**
		 * @brief Retrieves a block (struct data) that will be sent to a shader program.
		 * The block will contain all the light data.
		 *
		 * @param id the light id
		 * @return a data block for shader programs
		 */
		LightShaderBlock getLightInfo(const unsigned int &id);

		/**
		 * @brief Updates the light color vector.
		 *
		 * @param id the light id
		 * @param color the light new color
		 */
		void updateColor(const unsigned int &id, const glm::vec3 &color);

		/**
		 * @brief Updates the light intensity value.
		 *
		 * @param id the light id
		 * @param intensity the light new intensity
		 */
		void updateIntensity(const unsigned int &id, const float &intensity);

		/**
		 * @brief Updates the light ambient vector.
		 *
		 * @param id the light id
		 * @param ambient the light new ambient
		 */
		void updateAmbient(const unsigned int &id, const glm::vec3 &ambient);

		/**
		 * @brief Updates the light diffuse vector.
		 *
		 * @param id the light id
		 * @param diffuse the light new diffuse
		 */
		void updateDiffuse(const unsigned int &id, const glm::vec3 &diffuse);

		/**
		 * @brief Updates the light specular vector.
		 *
		 * @param id the light id
		 * @param specular the light new specular
		 */
		void updateSpecular(const unsigned int &id, const glm::vec3 &specular);

		/**
		 * @brief Updates the light direction vector.
		 *
		 * @param id the light id
		 * @param direction the light new direction
		 */
		void updateDirection(const unsigned int &id, const glm::vec3 &direction);

		/**
		 * @brief Updates the light position vector.
		 *
		 * @param id the light id
		 * @param position the light new position
		 */
		void updatePosition(const unsigned int &id, const glm::vec3 &position);

		/**
		 * @brief Updates the light constant value.
		 *
		 * @param id the light id
		 * @param constant the light new constant
		 */
		void updateConstant(const unsigned int &id, const float &constant);

		/**
		 * @brief Updates the light linear value.
		 *
		 * @param id the light id
		 * @param linear the light new linear
		 */
		void updateLinear(const unsigned int &id, const float &linear);

		/**
		 * @brief Updates the light quadratic value.
		 *
		 * @param id the light id
		 * @param quadratic the light new quadratic
		 */
		void updateQuadratic(const unsigned int &id, const float &quadratic);

		/**
		 * @brief Updates the light cutoff value.
		 *
		 * @param id the light id
		 * @param cutoff the light new cutoff
		 */
		void updateCutoff(const unsigned int &id, const float &cutoff);

		/**
		 * @brief Updates the light outer cutoff value.
		 *
		 * @param id the light id
		 * @param outerCutoff the light new outer cutoff
		 */
		void updateOuterCutoff(const unsigned int &id, const float &outerCutoff);

		/**
		 * @brief Updates the light computaion of an entity.
		 *
		 * @param id the light id
		 * @param comp the new entity light computation
		 */
		void setLightComputation(const unsigned int &id, const LightComputation &comp);

		/**
		 * @brief Retrieves the light computation of an entity.
		 *
		 * @param id the entity id
		 * @return the light computation
		 */
		LightComputation getLightComputation(const unsigned int &id);
	} // namespace light

	/**
	 * @namespace systems::render
	 * @brief All utilities for rendering entities.
	 */
	namespace render {
		/**
		 * @brief Initialize the stencil shader.
		 */
		void initStencilShader();

		/**
		 * @brief This methods calls the renderer and prepares the buffers with
		 * the model matrixes and colors.
		 *
		 * @param models a list of model matrixes
		 * @param colors a list of colors
		 *
		 * TODO implement proper instancing rendering for every mesh
		 * @warning This currently works only for instanced rendering with spheres.
		 */
		void prepareInstancedMesh(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors);

		/**
		 * @brief Retrieves all the light data and stores them in a shader block.
		 *
		 * It prepares a single light data for sending to the shader program.
		 *
		 * @param[out] block the shader light block
		 * @param[in] light the light in the scene
		 */
		void getOtherLightData(LightShaderBlock &block, const Shared<LightComponent> &light);

		/**
		 * @brief Prepares data of all lights in the scene and put them in an array with
		 * defined size.
		 *
		 * @return an array containing all the lights data prepared to be sent to shader programs.
		 */
		std::array<LightShaderBlock, SHADER_MAX_LIGHTS> prepareLightData();

		/**
		 * @brief This method sends to the used shader program the specific data for
		 * that light caster.
		 *
		 * @warning It doesn't send common light data.
		 * The method systems::render::sendOtherLightData() should be called anyway.
		 *
		 * @param shader the shader program shared pointer
		 * @param data the array processed with systems::render::prepareLightData()
		 * @param index the current light index in the array from systems::render::prepareLightData()
		 *
		 * @see systems::render::preparedLightData()
		 */
		void sendOtherLightData(const Shared<ogl::ShaderProgram> &shader, LightShaderBlock &data, size_t &index);

		/**
		 * @brief This method sends to the used shader program the common lights data.
		 *
		 * @warning It doesn't send data for specific casters.
		 * The method systems::render::sendOtherLightData() should be called anyway.
		 *
		 * @param shader the shader program shared pointer
		 * @param data the array processed with systems::render::preparedLightData()
		 *
		 * @see systems::render::preparedLightData()
		 */
		void sendLightDataShader(const Shared<ogl::ShaderProgram> &shader, std::array<LightShaderBlock, SHADER_MAX_LIGHTS> data);

		/**
		 * @brief This method draws the Skybox of the scene
		 *
		 * @param id the entity id
		 * @param shader the skybox shader program
		 */
		void renderSkybox(const unsigned int &id, const Shared<ogl::ShaderProgram> &shader);

		/**
		 * @brief Renders all the entities in the scene.
		 *
		 * It renderes all the entities with RenderComponent.
		 */
		void renderAllMeshes();

		/**
		 * @brief Renders all entities with instanced rendering available.
		 */
		void renderInstancedMeshes();

		/**
		 * @brief This method calculates the lines to be drawn from two corners
		 * of a ColliderComponent.
		 *
		 * @param botLeft the bot left corner position
		 * @param topRight the top right corner position
		 */
		std::vector<glm::vec3> getBoxLines(glm::vec3 &botLeft, glm::vec3 &topRight);

		/**
		 * @brief Renders all bounding box of the entities with a ColliderComponent.
		 */
		void renderBoundingBox();
	} // namespace render
} // namespace systems

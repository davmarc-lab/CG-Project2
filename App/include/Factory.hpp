#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Utils.hpp"

namespace factory {
	/**
	 * @brief Factory method to create a cube mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factoryCube(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	/**
	 * @brief Factory method to create a sphere mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factorySphere(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	/**
	 * @brief Factory method to create a sphere mesh in the ECS.
	 * This sphere is rendered using instance rendering.
	 * @see ogl::Renderer
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factorySphereInstanced(const BasicInfo &info, const glm::vec4 &color = {0, 0, 1, 1});

	/**
	 * @brief Factory method to create a pyramid mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factoryPyramid(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	/**
	 * @brief Factory method to create a thorus mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factoryThorus(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	/**
	 * @brief Factory method to create a cylinder mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param color the mesh color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factoryCylinder(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	/**
	 * @brief Factory method to create a rope mesh in the ECS.
	 *
	 * @param center the rope center position
	 * @param length the rope length
	 * @param constant the rope constant value
	 * @param subdivisions the rope subdivisions number (defines rope level of detail)
	 *
	 * @return a vector containing pairs of (color, modelMatrix) for each point of the rope
	 */
	std::vector<std::pair<glm::vec4, glm::mat4>> factoryRope(const glm::vec3 &center, const float &legnth, const float &constant, const unsigned int subdivisons);

	/**
	 * @brief Factory method to create a plane mesh in the ECS.
	 *
	 * @param color the plane color
	 *
	 * @return the id of the created entity
	 */
	unsigned int factoryPlane(const glm::vec4 &color);

	/**
	 * @brief Factory method to create a skybox in the scene.
	 *
	 * @param path the skybox images directory
	 * @param format the skybox image file format
	 *
	 * @return the id of the skybox
	 */
	unsigned int factorySkyBox(const std::string &path, const std::string &format);

	/**
	 * @brief Factory method to create an imported mesh in the ECS.
	 *
	 * @param info the mesh world data
	 * @param path the path to the `.obj` file to import
	 *
	 * @return the id of the imported mesh
	 */
	unsigned int factoryObjMesh(const BasicInfo &info, const std::string &path);

	/**
	 * @brief Factory method to create a tree in the ECS.
	 * It uses two pyramid and one cylinder with a standard texture.
	 *
	 * @param info the tree world data
	 *
	 * @return the id of the tree
	 */
	unsigned int factoryTree(const BasicInfo &info);

	namespace light {
		/**
		 * @brief Factory method to create a directional light.
		 *
		 * @param direction the light direction
		 *
		 * @return the id of the light
		 */
		unsigned int factoryDirectional(const glm::vec3 &direction);

		/**
		 * @brief Factory method to create a point light.
		 *
		 * @param position the light position
		 * @param constraint the light constraints data
		 *
		 * @return the id of the light
		 */
		unsigned int factoryPoint(const glm::vec3 &position, const ::light::LightConstraint &constraint);

		/**
		 * @brief Factory method to create a spot light.
		 *
		 * @param position the light position
		 * @param direction the light direction
		 * @param constraint the light constraints data
		 * @param cutOff the light cutoff value
		 * @param outerCutOff the light outer cutoff value
		 *
		 * @return the id of the light
		 */
		unsigned int factorySpot(const glm::vec3 &position, const glm::vec3 &direction, const ::light::LightConstraint &constraint, const float &cutOff = 12.5f, const float &outerCutOff = 17.5f);
	} // namespace light
} // namespace factory

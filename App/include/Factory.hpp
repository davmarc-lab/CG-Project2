#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Utils.hpp"

/**
 * @brief Simple data structure to store mesh information: position,
 * scale, rotation and if it needs to be rendered.
 */
struct BasicInfo {
	/// mesh position
	glm::vec3 position{};
	/// mesh scale
	glm::vec3 scale{1, 1, 1};
	/// mesh rotation
	glm::vec3 rotation{};
	/// render mesh flag
	bool render = true;
};

namespace factory {
	unsigned int factoryCube(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factorySphere(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factorySphereInstanced(const BasicInfo &info, const glm::vec4 &color = {0, 0, 1, 1});
	unsigned int factoryPyramid(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factoryThorus(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factoryCylinder(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

    std::vector<std::pair<glm::vec4, glm::mat4>> factoryRope(const glm::vec3& center, const float& legnth, const float& constant, const unsigned int subdivisons);

	unsigned int factoryPlane(const glm::vec4 &color);

	unsigned int factorySkyBox(const std::string &path, const std::string &format);

	unsigned int factoryObjMesh(const BasicInfo &info, const std::string &pathToFile);

	unsigned int factoryTree(const BasicInfo &info);

	namespace light {
		unsigned int factoryDirectional(const glm::vec3 &direction);
		unsigned int factoryPoint(const glm::vec3 &position, const LightConstraint &constraint);
		unsigned int factorySpot(const glm::vec3 &position, const glm::vec3 &direction, const LightConstraint &constraint, const float &cutOff = 12.5f, const float &outerCutOff = 17.5f);
	} // namespace light
} // namespace factory

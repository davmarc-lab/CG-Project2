#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Utils.hpp"

struct BasicInfo {
	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
};

namespace factory {
	unsigned int factoryCube(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factorySphere(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factoryPyramid(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});
	unsigned int factoryThorus(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

	unsigned int factoryPlane(const glm::vec4& color);

    unsigned int factorySkyBox(const std::string& path, const std::string& format);

	namespace light {
		unsigned int factoryDirectional(const glm::vec3 &direction);
		unsigned int factoryPoint(const glm::vec3 &position, const LightConstraint &constraint);
		unsigned int factorySpot(const glm::vec3 &position, const glm::vec3& direction, const LightConstraint &constraint, const float &cutOff = 12.5f, const float &outerCutOff = 17.5f);
	} // namespace light
} // namespace factory

#pragma once

#include <glm/glm.hpp>

struct BasicInfo {
	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
};

namespace factory {
	unsigned int factoryCube(const BasicInfo &info, const glm::vec4& color = {1, 0, 0, 1});
	unsigned int factorySphere(const BasicInfo &info, const glm::vec4& color = {1, 0, 0, 1});
	unsigned int factoryPyramid(const BasicInfo &info, const glm::vec4& color = {1, 0, 0, 1});
	unsigned int factoryThorus(const BasicInfo &info, const glm::vec4& color = {1, 0, 0, 1});
} // namespace factory

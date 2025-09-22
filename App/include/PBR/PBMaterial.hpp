#pragma once

#include "../ECS/Component.hpp"

class PBMaterial : public Component {
public:
	/// material name
	std::string name{"New Material"};

	glm::vec3 albedo{};
	float metallic{};
	float roughness{};
	float ao{};

	/**
	 * @brief Instances basic material.
	 */
	PBMaterial() = default;

	/**
	 * @brief Instances a material with the given data.
	 *
	 */
	PBMaterial(const glm::vec3 &albedo, const float &metallic, const float &roughness, const float &ao) :
		albedo(albedo), metallic(metallic), roughness(roughness), ao(ao), Component() {}

	/**
	 * @brief Overrides the equal operator, two materials are equal if they have
	 * the same name.
	 *
	 * @param other other material
	 */
	bool operator==(const Material &other) {
		return this->name == other.name;
	}
};

namespace pbr {
	const PBMaterial metal{{0.916, 0.923, 0.924}, 1, 0, 1};
}

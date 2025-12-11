#pragma once

#include "../ECS/Component.hpp"

/**
 * @brief This Component manages a pbr material.
 */
class PBMaterial : public Component {
public:
	/// material name
	std::string name{"New Material"};

    /// material albedo vector
	glm::vec3 albedo{};
    /// material metallic value
	float metallic{};
    /// material roughness value
	float roughness{};
    /// material ao value
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
    /// metal pbr material
	const PBMaterial metal{{0.5, 0, 0}, 1, 0.5, 0.1};
}

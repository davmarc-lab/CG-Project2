#pragma once

#include <glm/glm.hpp>
#include <string>

/**
 * @namespace light
 * Desc.
 *
 * TODO put everything of this file in the namespace light.
 */
namespace light {
	/*!
	 * @enum LightType
	 * @brief Lights type used in shader programs.
	 *
	 * This enum is needed to distinguish the lights in shader computation.
	 */
	enum LightType {
		LIGHT_DIRECTIONAL,
		LIGHT_POINT,
		LIGHT_SPOT,
	};
} // namespace light

// REMOVE THIS LINE
using namespace light;

#define SHADER_MAX_LIGHTS 32

/**
 * @brief Lights attenuation data structure.
 */
struct LightConstraint {
	/// light constant value
	float constant = 1.f;
	/// light linear value
	float linear = .09f;
	/// light quadratic value
	float quadratic = .032f;
};

/**
 * @brief Light vectors for light component.
 */
struct LightVectors {
	/// light ambient vector
	glm::vec3 ambient{.1f, .1f, .1f};
	/// light diffuse vector
	glm::vec3 diffuse{.8f, .8f, .8f};
	/// light specular vector
	glm::vec3 specular{1, 1, 1};
};

struct LightShaderBlock {
	int type = LightType::LIGHT_DIRECTIONAL;
	float intensity = 1;
	glm::vec3 color{1, 1, 1};

	glm::vec3 position{};
	glm::vec3 direction{};

	glm::vec3 ambient{.1f, .1f, .1f};
	glm::vec3 diffuse{.8f, .8f, .8f};
	glm::vec3 specular{1, 1, 1};

	float constant = 1;
	float linear = .09f;
	float quadratic = .032f;

	float cutoff = 12.5f;
	float outerCutoff = 17.5f;

	bool isSmooth = false;
};

enum LightComputation {
	NONE,
	PHONG,
	BLINN_PHONG,
	INT_PHONG,
	INT_BLINN_PHONG,
};

unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels = 0);

void flipImagesVertically(const bool &val);

void freeImageData(void *data);

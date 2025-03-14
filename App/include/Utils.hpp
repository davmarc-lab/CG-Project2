#pragma once

#include <string>
#include <glm/glm.hpp>

enum LightType {
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
};

#define SHADER_MAX_LIGHTS 32

struct LightConstraint {
	float constant = 1.f, linear = .09f, quadratic = .032f;
};

struct LightVectors {
	glm::vec3 ambient{.1f, .1f, .1f};
	glm::vec3 diffuse{.8f, .8f, .8f};
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

unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels = 0);

void flipImagesVertically(const bool &val);

void freeImageData(void *data);

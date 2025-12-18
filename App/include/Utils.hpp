#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../../Opengl-Core/include/Core.hpp"

/**
 * @enum InputState
 * @brief It tells which state is the mouse.
 *
 * If the mouse in passive state means that the camera will move following
 * the mouse cursor, if the state is active the camera will not follow
 * the cursor.
 */
enum InputState {
	MOUSE_PASSIVE,
	MOUSE_ACTIVE
};

/**
 * @brief Data structure to store mouse position data and allow fluent movement.
 */
struct Mouse {
	/// mouse current position
	glm::vec2 pos{};
	/// first move of the cursor
	bool first = true;
	/// if true start executing trackball movement
	bool trackState = false;
	/// skips the current position if the cursor has been moved to the opposite side
	/// of the window
	bool skipCursorPos = false;
};

/**
 * @brief Register callbacks for the WorldCamera movement with collisions.
 *
 * @param world the world camera
 */
void defaultCameraMovement(ogl::WorldCamera &world);

/**
 * @brief Utility method that manages mouse behaviour in active or passive mode.
 * It changes the GLFW callbacks.
 *
 * @param w the window
 * @param world the world camera
 * @param state the current input state (ACTIVE - PASSIVE)
 * @param mouse the mouse data
 */
void changeInputState(ogl::Window *w, ogl::WorldCamera &world, const InputState &state, Mouse &mouse);

/**
 * @brief Defines default callbacks for a standard Window.
 *
 * @param w the window
 * @param world the world camera
 * @param mouse the mouse data
 */
void defaultKeyCallback(ogl::Window *w, ogl::WorldCamera &world, Mouse &mouse);

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

/**
 * @namespace light
 * Desc.
 */
namespace light {
	/*!
	 * @enum LightType
	 * @brief Lights type used in shader programs.
	 *
	 * This enum is needed to distinguish the lights in shader computation.
	 */
	enum LightType {
		/// directional light
		LIGHT_DIRECTIONAL,
		/// point light
		LIGHT_POINT,
		/// spot light
		LIGHT_SPOT,
	};

/// max lights in shaders
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

	/**
	 * @brief Data structure to send light data to shaders.
	 */
	struct LightShaderBlock {
		/// light type
		int type = LightType::LIGHT_DIRECTIONAL;
		/// light intensity
		float intensity = 1;
		/// light color
		glm::vec3 color{1, 1, 1};

		/// light position
		glm::vec3 position{};
		/// light direction
		glm::vec3 direction{};

		/// light ambient vector
		glm::vec3 ambient{.1f, .1f, .1f};
		/// light diffuse vector
		glm::vec3 diffuse{.8f, .8f, .8f};
		/// light specular vector
		glm::vec3 specular{1, 1, 1};

		/// light constant value
		float constant = 1;
		/// light linear value
		float linear = .09f;
		/// light quadratic value
		float quadratic = .032f;

		/// light cutoff value
		float cutoff = 12.5f;
		/// light outer cutoff value
		float outerCutoff = 17.5f;

		/// light smooth flag
		bool isSmooth = false;
	};

	/**
	 * @enum LightComputation
	 * @brief Defines light alghoritm to use in shaders.
	 */
	enum LightComputation : unsigned int {
		/// no light computation
		NONE = 0,
		/// Phong alghoritm computation
		PHONG,
		/// Blinn-Phong alghoritm computation
		BLINN_PHONG,
		/// Interpolative Phong alghoritm computation
		INT_PHONG,
		/// Interpolative Blinn-Phong alghoritm computation
		INT_BLINN_PHONG,
	};
} // namespace light

/**
 * @brief Read data from the given image.
 *
 * @param path the image path
 * @param width the image width
 * @param height the image height
 * @param nrChannels the image channels number
 * @param desiredChannels the number of desired channels
 *
 * @return pointer to the data read from image
 */
unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels = 0);

/**
 * @brief Tells to the stbi library to flip images vertically.
 *
 * @param val the flag to flip
 */
void flipImagesVertically(const bool &val);

/**
 * @brief Frees the data read from an image.
 *
 * @param data the pointer to image data
 */
void freeImageData(void *data);

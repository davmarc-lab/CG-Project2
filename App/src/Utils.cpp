#include "../include/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <string>

#include "../include/stb_image.hpp"

#include "../include/AppGui.hpp"
#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../../Opengl-Core/include/Core.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto em = EntityManager::instance();
const auto im = InputManager::instance();

void defaultCameraMovement(WorldCamera &world) {
	// register an event during loop input step
	ed->subscribe(event::loop::LOOP_INPUT, [world]() {
		// take camera collider
		auto collider = em->getComponentFromId<ColliderComponent>(world.cameraId);
		if (im->isKeyPressed(GLFW_KEY_W)) {
			// move the camera when the key is pressed
			world.camera->moveCamera(world.camera->getCameraFront());
            // post camera update event
			ed->post(CAMERA_UPDATE_DATA);
            // manage collision only if the camera has a collider
			if (collider != nullptr)
                // retrieves all collisions, if the camera is in this vector then it's colliding
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
                        // restore old position (basically no movement)
						world.camera->moveCamera(-world.camera->getCameraFront());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
		if (im->isKeyPressed(GLFW_KEY_S)) {
			world.camera->moveCamera(-world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
			if (collider != nullptr)
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
						world.camera->moveCamera(world.camera->getCameraFront());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
		if (im->isKeyPressed(GLFW_KEY_D)) {
			world.camera->moveCamera(world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
			if (collider != nullptr)
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
						world.camera->moveCamera(-world.camera->getCameraRight());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
		if (im->isKeyPressed(GLFW_KEY_A)) {
			world.camera->moveCamera(-world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
			if (collider != nullptr)
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
						world.camera->moveCamera(world.camera->getCameraRight());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
		if (im->isKeyPressed(GLFW_KEY_SPACE)) {
			world.camera->moveCamera(world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
			if (collider != nullptr)
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
						world.camera->moveCamera(-world.camera->getCameraUp());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
		if (im->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			world.camera->moveCamera(-world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
			if (collider != nullptr)
				for (auto [first, second] : systems::collision::getCollisions()) {
					if (first == world.cameraId || second == world.cameraId) {
						world.camera->moveCamera(world.camera->getCameraUp());
						ed->post(CAMERA_UPDATE_DATA);
					}
				}
		}
	});
}

void changeInputState(Window *w, WorldCamera &world, const InputState &state, Mouse &mouse) {
	switch (state) {
		case MOUSE_PASSIVE: {
            // every time mouse is passive restore first input flag
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            // disable cursor in passive mode to avoid moving manually the pointer to the center
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // define cursor callback
			w->setCursorPosCallback([world, &mouse](GLFWwindow *window, double x, double y) {
				if (mouse.first) {
					mouse.first = false;
					mouse.pos = {x, y};
					return;
				}

				auto xoffset = x - mouse.pos.x;
				auto yoffset = y - mouse.pos.y;
				mouse.pos = {x, y};

                // process cursor position
				world.camera->processMouseMovement(xoffset, yoffset);
			});
			break;
		}
		case MOUSE_ACTIVE: {
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            // enable the mouse pointer
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

            // delete cursor position callback
			w->setCursorPosCallback([](auto, auto, auto) {});
			break;
		}
	}
}

void defaultKeyCallback(Window *w, WorldCamera &world, Mouse &mouse) {
    // standard key callback to work with ogl::InputManager
	w->setKeysCallback([w, &world, &mouse](GLFWwindow *window, int key, int code, int action, int mod) {
		switch (action) {
			case GLFW_REPEAT:
			case GLFW_PRESS: {
				im->keyPressed(key);
				break;
			}
			case GLFW_RELEASE:
				im->keyReleased(key);
				break;
			default:
				break;
		}
        // other keys callback
		if (key == GLFW_KEY_P) {
			changeInputState(w, world, MOUSE_PASSIVE, mouse);
			return;
		}
		if (key == GLFW_KEY_I) {
			changeInputState(w, world, MOUSE_ACTIVE, mouse);
			return;
		}
	});
}

unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels) {
    // load the given image using stbi library
	return stbi_load(path.c_str(), &width, &height, &nrChannels, desiredChannels);
}

void flipImagesVertically(const bool &val) {
    // flip the image
	stbi_set_flip_vertically_on_load(val);
}

void freeImageData(void *data) {
    // free image data
	stbi_image_free(data);
}

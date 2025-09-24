#include "../include/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <string>

#include "../include/stb_image.hpp"

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"
#include "../include/AppGui.hpp"

#include "../../Opengl-Core/include/Core.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto em = EntityManager::instance();
const auto im = InputManager::instance();

void defaultCameraMovement(WorldCamera &world) {
	ed->subscribe(event::loop::LOOP_INPUT, [world]() {
		auto collider = em->getComponentFromId<ColliderComponent>(world.cameraId);
		ASSERT(collider != nullptr);

		bool collision = false;
		if (im->isKeyPressed(GLFW_KEY_W)) {
			world.camera->moveCamera(world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == world.cameraId || second == world.cameraId) {
					world.camera->moveCamera(-world.camera->getCameraFront());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_S)) {
			world.camera->moveCamera(-world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
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
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			w->setCursorPosCallback([world, &mouse](GLFWwindow *window, double x, double y) {
				if (mouse.first) {
					mouse.first = false;
					mouse.pos = {x, y};
					return;
				}

				auto xoffset = x - mouse.pos.x;
				auto yoffset = y - mouse.pos.y;

				mouse.pos = {x, y};

				world.camera->processMouseMovement(xoffset, yoffset);
			});
			break;
		}
		case MOUSE_ACTIVE: {
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

			w->setCursorPosCallback([](auto, auto, auto) {});
			break;
		}
	}
}

void defaultKeyCallback(Window *w, WorldCamera &world, Mouse& mouse) {
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
	return stbi_load(path.c_str(), &width, &height, &nrChannels, desiredChannels);
}

void flipImagesVertically(const bool &val) {
	stbi_set_flip_vertically_on_load(val);
}

void freeImageData(void *data) {
	stbi_image_free(data);
}

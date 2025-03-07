#include "../../Opengl-Core/include/Core.hpp"

#include "../include/ECS/EcsScene.hpp"
#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../include/AppGui.hpp"

#include "../include/Factory.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ogl;

const auto ed = EventManager::instance();
const auto im = InputManager::instance();
const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

const auto ENTITY_ELECTED_CHANGED = Event("Entity Selected Changed");

void enableDefaultCameraMovement() {
	ed->subscribe(event::loop::LOOP_INPUT, []() {
		if (im->isKeyPressed(GLFW_KEY_W)) {
			camera.moveCamera(camera.getCameraVelocity() * camera.getCameraFront());
		}

		if (im->isKeyPressed(GLFW_KEY_S)) {
			camera.moveCamera(camera.getCameraVelocity() * -camera.getCameraFront());
		}

		if (im->isKeyPressed(GLFW_KEY_D)) {
			camera.moveCamera(camera.getCameraVelocity() * camera.getCameraRight());
		}

		if (im->isKeyPressed(GLFW_KEY_A)) {
			camera.moveCamera(camera.getCameraVelocity() * -camera.getCameraRight());
		}

		if (im->isKeyPressed(GLFW_KEY_SPACE)) {
			camera.moveCamera(camera.getCameraVelocity() * camera.getCameraUp());
		}

		if (im->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			camera.moveCamera(camera.getCameraVelocity() * -camera.getCameraUp());
		}
	});
}

enum InputState {
	MOUSE_PASSIVE,
	MOUSE_ACTIVE
};

struct Mouse {
	glm::vec2 pos{};
	bool first = true;
} mouse;

glm::vec3 getRayFromMouse(const Pair<float> &size, int mouse_x, int mouse_y) {
	mouse_y = size.y - mouse_y;

	float ndc_x = (2.0f * mouse_x) / size.x - 1.0f;
	float ndc_y = (2.0f * mouse_y) / size.y - 1.0f;
	float ndc_z = -1.0f;

	auto clip = glm::vec4(ndc_x, ndc_y, ndc_z, 1.0f);

	auto view_model_p = glm::inverse(camera.getProjMatrix()) * clip;
	view_model_p.w = 1;

	auto pw = glm::inverse(camera.getViewMatrix()) * view_model_p;

	return glm::normalize(glm::vec3(pw) - glm::vec3(camera.getCameraPosition()));
}

bool isRayInSphere(const glm::vec3 &ray, const glm::vec3 &sphere_pos, const float &sphere_radius, float *id) {
	glm::vec3 d = camera.getCameraPosition() - sphere_pos;
	float b = dot(d, ray);
	float cc = dot(d, d) - sphere_radius * sphere_radius;
	float delta = b * b - cc;

	if (delta < 0.0f)
		return false;
	else if (delta > 0.0f) {
		float t_a = -b + sqrt(delta);
		float t_b = -b - sqrt(delta);
		*id = t_b;

		if (t_a < 0.0f && t_b < 0.0f)
			return false;

		return true;
	} else { // delta == 0.0f
		float t = -b + sqrt(delta);
		if (t < 0)
			return false;
		*id = t;
		return true;
	}
}

int ettSelected = -1;

void changeInputState(Window &w, const InputState &state) {
	switch (state) {
		case MOUSE_PASSIVE: {
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w.getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			w.setMouseButtonCallback([](GLFWwindow *, int, int, int) {});
			w.setCursorPosCallback([](GLFWwindow *window, double x, double y) {
				if (mouse.first) {
					mouse.first = false;
					mouse.pos = {x, y};
					return;
				}

				auto xoffset = x - mouse.pos.x;
				auto yoffset = y - mouse.pos.y;

				mouse.pos = {x, y};

				camera.processMouseMovement(xoffset, yoffset);
			});
			break;
		}
		case MOUSE_ACTIVE: {
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w.getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
			w.setMouseButtonCallback([&w](GLFWwindow *window, int button, int action, int mods) {
				switch (button) {
					case GLFW_MOUSE_BUTTON_1: {
						if (action == GLFW_PRESS) {
							double x, y;
							glfwGetCursorPos(window, &x, &y);
							float xmouse = x;
							float ymouse = y;

							glm::vec3 ray = getRayFromMouse(w.getSize(), xmouse, ymouse);
							// closest_intersection
							float ci = 0;
							// this pointer keeps track of the selected object
							ettSelected = -1;

							// distance between ray and object
							float dist = 0.f;

							// check all entities of the scene for intersection with ray
							for (auto elem : em->getEntitiesFromComponent<Transform>()) {
								if (isRayInSphere(ray, systems::transform::getPosition(elem), 1, &dist)) {
									if (ettSelected == -1 || dist <= ci) {
										ettSelected = elem;
										ci = dist;
									}
								}
							}
						}
						ed->post(ENTITY_ELECTED_CHANGED);
						break;
					}
				}
			});
			w.setCursorPosCallback([](GLFWwindow *window, double x, double y) {});
			break;
		}
		default:
			break;
	}
}

int main(int argc, char *argv[]) {
	WindowSettings s{};
	s.decorated = false;
	s.size = {1366, 768};
	s.position = {400, 12};
	s.focused = true;

	Window w{s};
	w.onAttach();
	w.setKeysCallback([&w](GLFWwindow *window, int key, int code, int action, int mod) {
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
			changeInputState(w, MOUSE_PASSIVE);
			return;
		}
		if (key == GLFW_KEY_I) {
			changeInputState(w, MOUSE_ACTIVE);
			return;
		}
	});

	changeInputState(w, MOUSE_ACTIVE);

	glEnable(GL_CULL_FACE);
	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	enableDefaultCameraMovement();
	camera.updatePerspProjection(45.f, w.getWidth(), w.getHeight(), 0.1f, 100.f);

	Renderer::instance()->init();

	ImGuiManager im{"ImGui Manager", w, DEFAULT_IMGUI_CONFIGS | ImGuiConfigFlags_ViewportsEnable};
	im.onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	im.addPanel<ImGuiEntityTree>();
	auto igEttModel = im.addPanel<ImGuiEntityModel>();

	/*
		FrameBuffer fb{{GL_FRAMEBUFFER, 100, 100}};
		fb.onAttach();

		fb.setRenderBuffer({GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 100, 100});

		Texture t = Texture({GL_TEXTURE_2D, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE}, 100, 100);
		t.onAttach();
		t.createTexture2D(NULL);
		t.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		t.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		fb.setColorTexture(t);
		fb.createFrameBuffer();
	*/

	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();
	Shared<ShaderProgram> normalShader = CreateShared<ShaderProgram>("normalVertShader.glsl", "normalFragShader.glsl", "normalGeomShader.glsl");
	normalShader->createShaderProgram();

	auto shape = factory::factoryCube(BasicInfo{{1, 1, -3}, {1, 1, 1}, {}});
	scene->addEntity(shader, shape);

	auto pyr = factory::factoryPyramid(BasicInfo{{-1, 1, -3}, {1, 1, 1}, {}});
	scene->addEntity(shader, pyr);

	UniformBuffer ub("Matrices");
	ub.onAttach();
	ub.setup(sizeof(glm::mat4), 0, 0, 0);
	auto viewProj = camera.getViewProjMatrix();
	ub.update(0, sizeof(glm::mat4), glm::value_ptr(viewProj));
	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&ub]() {
		auto vp = camera.getViewProjMatrix();
		ub.update(0, sizeof(glm::mat4), glm::value_ptr(vp));
	});

	ed->subscribe(event::loop::LOOP_UPDATE, [&shape]() {
		// systems::transform::addRotation(shape, {2, 0, 0});
	});

	ed->subscribe(ENTITY_ELECTED_CHANGED, [&igEttModel]() {
		igEttModel->setSelectedEntity(ettSelected);
	});

	ed->subscribe(event::loop::LOOP_RENDER, [&normalShader, &shape]() {
		systems::render::renderAllMeshes();
		normalShader->use();
		auto p = camera.getProjMatrix();
		auto v = camera.getViewMatrix();
		normalShader->setMat4("view", v);
		normalShader->setMat4("proj", p);
		normalShader->setMat4("model", systems::transform::getModelMatrix(shape));
		auto rc = em->getComponentFromId<RenderComponent>(shape);
		rc->call();
	});

	while (!glfwWindowShouldClose(w.getContext())) {
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}

	w.onDetach();

	return 0;
}

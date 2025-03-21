#include "../../Opengl-Core/include/Core.hpp"

#include "../include/ECS/EcsScene.hpp"
#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../include/AppGui.hpp"

#include "../include/Factory.hpp"

#include <GLFW/glfw3.h>
#include <glm/exponential.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

using namespace ogl;

const auto ed = EventManager::instance();
const auto im = InputManager::instance();
const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

const auto ENTITY_ELECTED_CHANGED = Event("Entity Selected Changed");

int ettSelected = -1;

struct WorldCamera {
	unsigned int cameraId;
	Shared<Camera> camera;
	glm::vec3 cameraSize = glm::vec3(1);
	int tbBorderTolerance = 20;
	float tbRadius = 1.f;
} world;

enum InputState {
	MOUSE_PASSIVE,
	MOUSE_ACTIVE
};

struct Mouse {
	glm::vec2 pos{};
	bool first = true;
	bool trackState = false;
	bool skipCursorPos = false;
} mouse;

void enableDefaultCameraMovement() {
	ed->subscribe(event::loop::LOOP_INPUT, []() {
		if (im->isKeyPressed(GLFW_KEY_W)) {
			world.camera->moveCamera(world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
		}
		if (im->isKeyPressed(GLFW_KEY_S)) {
			world.camera->moveCamera(-world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
		}
		if (im->isKeyPressed(GLFW_KEY_D)) {
			world.camera->moveCamera(world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
		}
		if (im->isKeyPressed(GLFW_KEY_A)) {
			world.camera->moveCamera(-world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
		}
		if (im->isKeyPressed(GLFW_KEY_SPACE)) {
			world.camera->moveCamera(world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
		}
		if (im->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			world.camera->moveCamera(-world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
		}
	});
}

glm::vec3 getTrackballPoint(const Pair<float> &viewpSize, const glm::vec2 &pos) {
	glm::vec3 point{};
	glm::vec3 offset{};
	point.x = (2 * (pos.x) - viewpSize.x) / viewpSize.x;
	point.y = (viewpSize.y - 2 * (pos.y)) / viewpSize.y;

	auto zTmp = world.tbRadius - pow(point.x, 2) - pow(point.y, 2);
	point.z = zTmp < 0 ? 0 : sqrt(zTmp);
	return glm::normalize(point);
}

glm::vec3 getRayFromMouse(const Pair<float> &size, int mouse_x, int mouse_y) {
	mouse_y = size.y - mouse_y;

	float ndc_x = (2.0f * mouse_x) / size.x - 1.0f;
	float ndc_y = (2.0f * mouse_y) / size.y - 1.0f;
	float ndc_z = -1.0f;

	auto clip = glm::vec4(ndc_x, ndc_y, ndc_z, 1.0f);

	auto view_model_p = glm::inverse(world.camera->getProjMatrix()) * clip;
	view_model_p.w = 1;

	auto pw = glm::inverse(world.camera->getViewMatrix()) * view_model_p;

	return glm::normalize(glm::vec3(pw) - glm::vec3(world.camera->getCameraPosition()));
}

bool isRayInSphere(const glm::vec3 &ray, const glm::vec3 &sphere_pos, const float &sphere_radius, float *id) {
	glm::vec3 d = world.camera->getCameraPosition() - sphere_pos;
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
	} else {
		float t = -b + sqrt(delta);
		if (t < 0)
			return false;
		*id = t;
		return true;
	}
}

void changeInputState(Window &w, const InputState &state) {
	switch (state) {
		case MOUSE_PASSIVE: {
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w.getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			w.setMouseButtonCallback([](GLFWwindow *, int, int, int) {
			});
			w.setCursorPosCallback([](GLFWwindow *window, double x, double y) {
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
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w.getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
			w.setCursorPosCallback([&w](GLFWwindow *window, double x, double y) {
				if (mouse.skipCursorPos) {
					mouse.skipCursorPos = false;
					return;
				}
				if (mouse.first) {
					mouse.first = false;
					mouse.pos = {x, y};
				}
				if (!mouse.trackState) {
					mouse.pos = {x, y};
					return;
				}

				auto pre = getTrackballPoint(w.getSize(), mouse.pos);
				auto current = getTrackballPoint(w.getSize(), {x, y});

				auto dpos = current - pre;
				if (dpos.x || dpos.y || dpos.z) {
					auto speed = world.camera->getTrackballSpeed();
					// lenght of the new point.
					auto dist = glm::dot(pre, current);
					// avoid out of bound calculations
					if (dist > 1) {
						dist = 1;
					} else if (dist < -1) {
						dist = -1;
					}
					auto angle = glm::acos(dist) * speed;
					glm::vec3 rotAxis = glm::cross(pre, current);
					world.camera->setCameraDirection(world.camera->getCameraPosition() - world.camera->getCameraTarget());
					world.camera->setCameraPosition(glm::vec4(world.camera->getCameraTarget(), 0) + glm::rotate(glm::mat4(1.f), glm::radians(-angle), rotAxis) * glm::vec4(world.camera->getCameraDirection(), 0));
				}
				mouse.pos = {x, y};
				if (mouse.pos.x < world.tbBorderTolerance) {
					mouse.pos.x = w.getWidth() - world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.x > w.getWidth() - world.tbBorderTolerance) {
					mouse.pos.x = world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.y < world.tbBorderTolerance) {
					mouse.pos.y = w.getHeight() - world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.y > w.getHeight() - world.tbBorderTolerance) {
					mouse.pos.y = world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
			});
			w.setMouseButtonCallback([&w](GLFWwindow *window, int button, int action, int mods) {
				switch (button) {
					case GLFW_MOUSE_BUTTON_3: {
						mouse.trackState = (action == GLFW_PRESS);
						break;
					}
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
							ed->post(ENTITY_ELECTED_CHANGED);
						}
						break;
					}
				}
			});
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
#ifdef _WIN32
	s.position = {470, 50};
#endif
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

	Renderer::instance()->init();

	ImGuiManager im{"ImGui Manager", w, DEFAULT_IMGUI_CONFIGS | ImGuiConfigFlags_ViewportsEnable};
	im.onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	im.addPanel<ImGuiEntityTree>();
	auto igEttModel = im.addPanel<ImGuiEntityModel>();

	// Setting up the camera
	world.cameraId = em->createEntity();
	systems::ecs::updateEntityName(world.cameraId, "Main Camera");
	auto cam = em->addComponent<CameraComponent>(world.cameraId);
	cam->camera = CreateShared<ogl::Camera>();
	world.camera = systems::camera::getCamera(world.cameraId);
	em->addComponent<ColliderComponent>(world.cameraId, glm::vec3{4, 4, 4}, world.cameraSize);
	ed->post(CAMERA_UPDATE_DATA);
	enableDefaultCameraMovement();
	world.camera->updatePerspProjection(world.camera->getCameraZoom(), w.getWidth(), w.getHeight(), 0.1f, 100.f);

	ed->subscribe(CAMERA_UPDATE_DATA, []() {
		systems::camera::updateCameraCollider(world.cameraId, world.camera->getCameraPosition(), world.cameraSize);
	});

	// Initializing Scene
	scene->init(world.camera);

	Shared<ShaderProgram> skyboxShader = CreateShared<ShaderProgram>("skyboxVertShader.glsl", "skyboxFragShader.glsl");
	skyboxShader->createShaderProgram();
	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();
	Shared<ShaderProgram> lightShader = CreateShared<ShaderProgram>("lightVertShader.glsl", "lightFragShader.glsl");
	lightShader->createShaderProgram();
	Shared<ShaderProgram> normalShader = CreateShared<ShaderProgram>("normalVertShader.glsl", "normalFragShader.glsl", "normalGeomShader.glsl");
	normalShader->createShaderProgram();

	auto plane = factory::factoryPlane({0.3, 0.3, 0.3, 1});
	systems::ecs::updateEntityName(plane, "Basic Plane");
	scene->addEntity(shader, plane);

	auto skybox = factory::factorySkyBox("./resources/texture/skybox/sea/", "jpg");

	auto shape = factory::factoryCube(BasicInfo{{1, 1, -3}, {1, 1, 1}, {}});
	scene->addEntity(lightShader, shape);
	em->addComponent<MaterialComponent>(shape);
	em->addComponent<ParentComponent>(shape);
	em->addComponent<ColliderComponent>(shape);
	systems::ecs::updateEntityName(shape, "Cube");

	TextureParams params{};
	params.target = GL_TEXTURE_2D;
	params.internalFormat = GL_RGB;
	params.format = GL_RGB;
	params.dataType = GL_UNSIGNED_BYTE;
	int width, height, nrChannels;
	flipImagesVertically(true);
	auto data = readImageData("./resources/texture/woddenContainer.jpg", width, height, nrChannels);
	ogl::Texture t{params, {(unsigned int)width, (unsigned int)height}};
	t.onAttach();
	t.bind();
	t.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	t.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	t.setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	t.setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	t.createTexture2D(data);
	t.generateMipmap();
	em->addComponent<TextureComponent>(shape, "./resources/texture/woddenContainer.jpg");
	systems::texture::setTexture(shape, t);
	freeImageData(data);
	t.unbind();

	auto pyr = factory::factoryThorus(BasicInfo{{-1, 1, -3}, {1, 1, 1}, {}});
	em->addComponent<MaterialComponent>(pyr);
	em->addComponent<ColliderComponent>(pyr);

	data = readImageData("./resources/texture/dirt.jpg", width, height, nrChannels);
	ogl::Texture pt{params, {(unsigned int)width, (unsigned int)height}};
	pt.onAttach();
	pt.bind();
	pt.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	pt.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	pt.setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	pt.setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	pt.createTexture2D(data);
	pt.generateMipmap();
	em->addComponent<TextureComponent>(pyr, "./resources/texture/dirt.jpg");
	systems::texture::setTexture(pyr, pt);
	freeImageData(data);
	scene->addEntity(lightShader, pyr);
	pt.unbind();
	systems::texture::setMeshReflective(shape, true);

    auto obj = factory::factoryObjMesh(BasicInfo{}, "./resources/models/backpack/backpack.obj");
    systems::ecs::updateEntityName(obj, "backpack");

	auto id = factory::light::factoryDirectional({1, 0, 0});

	UniformBuffer ub("Matrices");
	ub.onAttach();
	ub.setup(sizeof(glm::mat4), 0, 0, 0);
	auto viewProj = world.camera->getViewProjMatrix();
	ub.update(0, sizeof(glm::mat4), glm::value_ptr(viewProj));
	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&ub]() {
		auto vp = world.camera->getViewProjMatrix();
		ub.update(0, sizeof(glm::mat4), glm::value_ptr(vp));
	});

	ed->subscribe(event::loop::LOOP_UPDATE, [&pyr]() {
		systems::transform::addRotation(pyr, {2, 1, 0});
	});

	ed->subscribe(event::loop::LOOP_UPDATE, []() { auto coll = systems::collision::getCollisions(); });

	ed->subscribe(ENTITY_ELECTED_CHANGED, [&igEttModel]() {
		igEttModel->setSelectedEntity(ettSelected);
	});

	ed->subscribe(event::loop::LOOP_RENDER, [&normalShader, &skyboxShader, &shape, &skybox]() {
		// render skybox
		systems::render::renderSkybox(skybox, skyboxShader);
		// render other meshes
		systems::render::renderAllMeshes();
		systems::render::renderBoundingBox();
		// normalShader->use();
		// auto p = world.cam.getProjMatrix();
		// auto v = world.cam.getViewMatrix();
		// normalShader->setMat4("view", v);
		// normalShader->setMat4("proj", p);
		// normalShader->setMat4("model", systems::transform::getModelMatrix(shape));
		// auto rc = em->getComponentFromId<RenderComponent>(shape);
		// rc->call();
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

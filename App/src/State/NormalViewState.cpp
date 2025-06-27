#include "../../include/State/NormalViewState.hpp"

#include "../../include/AppGui.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/EntityManager.hpp"
#include "../../include/ECS/System.hpp"
#include "../../include/Factory.hpp"
#include "../../include/Profiler.hpp"

#include <glm/gtc/type_ptr.hpp>

const auto em = EntityManager::instance();
const auto im = InputManager::instance();
const auto ed = EventManager::instance();
const auto scene = BasicScene::instance();

void NormalViewState::enableDefaultCameraMovement() {
	ed->subscribe(event::loop::LOOP_INPUT, [this]() {
		auto collider = em->getComponentFromId<ColliderComponent>(world.cameraId);
		ASSERT(collider != nullptr);

		bool collision = false;
		if (im->isKeyPressed(GLFW_KEY_W)) {
			world.camera->moveCamera(world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == world.cameraId || second == world.cameraId) {
					if (first == plane || second == plane) {
						continue;
					}
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
					if (first == plane || second == plane) {
						continue;
					}
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
					if (first == plane || second == plane) {
						continue;
					}
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
					if (first == plane || second == plane) {
						continue;
					}
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
					if (first == plane || second == plane) {
						continue;
					}
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
					if (first == plane || second == plane) {
						continue;
					}
					world.camera->moveCamera(world.camera->getCameraUp());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
	});
}

glm::vec3 NormalViewState::getTrackballPoint(const Pair<float> &viewpSize, const glm::vec2 &pos) {
	glm::vec3 point{};
	glm::vec3 offset{};
	point.x = (2 * (pos.x) - viewpSize.x) / viewpSize.x;
	point.y = (viewpSize.y - 2 * (pos.y)) / viewpSize.y;

	auto zTmp = world.tbRadius - pow(point.x, 2) - pow(point.y, 2);
	point.z = zTmp < 0 ? 0 : sqrt(zTmp);
	return glm::normalize(point);
}

glm::vec3 NormalViewState::getRayFromMouse(const Pair<float> &size, int mouse_x, int mouse_y) {
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

bool NormalViewState::isRayInSphere(const glm::vec3 &ray, const glm::vec3 &sphere_pos, const float &sphere_radius, float *id) {
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

void NormalViewState::changeInputState(Window *w, const InputState &state) {
	switch (state) {
		case MOUSE_PASSIVE: {
			mouse.first = true;
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			w->setMouseButtonCallback([](GLFWwindow *, int, int, int) {
			});
			w->setCursorPosCallback([this](GLFWwindow *window, double x, double y) {
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
				glfwSetInputMode(w->getContext(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(w->getContext(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
			w->setCursorPosCallback([this, w](GLFWwindow *window, double x, double y) {
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

				auto pre = this->getTrackballPoint(w->getSize(), mouse.pos);
				auto current = this->getTrackballPoint(w->getSize(), {x, y});

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
					mouse.pos.x = w->getWidth() - world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.x > w->getWidth() - world.tbBorderTolerance) {
					mouse.pos.x = world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.y < world.tbBorderTolerance) {
					mouse.pos.y = w->getHeight() - world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
				if (mouse.pos.y > w->getHeight() - world.tbBorderTolerance) {
					mouse.pos.y = world.tbBorderTolerance;
					mouse.skipCursorPos = true;
					glfwSetCursorPos(window, mouse.pos.x, mouse.pos.y);
				}
			});
			w->setMouseButtonCallback([this, w](GLFWwindow *window, int button, int action, int mods) {
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

							glm::vec3 ray = getRayFromMouse(w->getSize(), xmouse, ymouse);
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

void NormalViewState::defaultKeyCallback(Window *w) {
	w->setKeysCallback([this, w](GLFWwindow *window, int key, int code, int action, int mod) {
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
}

Unique<Window> w = nullptr;
Unique<ImGuiManager> igm = nullptr;

class CustomLayer : public Layer {
public:
	virtual void onAttach() override {}
	virtual void onDetach() override {}

	virtual void onUpdate() override {
		if (this->m_run)
			this->m_updateFun();
	}

	void setRunnig(const bool &run) { this->m_run = run; }
	void setUpdateFun(std::function<void()> &&fun) { this->m_updateFun = std::move(fun); }

	CustomLayer() :
		Layer("Custom Layer") {}

	virtual ~CustomLayer() override = default;

private:
	bool m_run = false;
	std::function<void()> m_updateFun{};
};

void NormalViewState::onAttach() {
    ASSERT(!this->m_attached);
	State::onAttach();
	srand(time(NULL));
	WindowSettings s{};
	s.decorated = false;
	s.size = {1366, 768};
	s.position = {400, 12};

#ifdef BIG
	s.position = {10, 606};
#endif // BIG
#ifdef _WIN32
	s.position = {470, 50};
#endif
	s.focused = true;

	w = CreateUnique<Window>(s);
	ASSERT(w != nullptr);
	w->onAttach();
	defaultKeyCallback(w.get());
	changeInputState(w.get(), MOUSE_ACTIVE);

	glEnable(GL_CULL_FACE);
	ed->subscribe(event::loop::LOOP_UPDATE, []() { w->onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, []() { w->onRender(); });

	Renderer::instance()->init();

	igm = CreateUnique<ImGuiManager>("ImGui Manager", w.get(), DEFAULT_IMGUI_CONFIGS | ImGuiConfigFlags_ViewportsEnable);
	igm->onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, []() { igm->onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, []() { igm->onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, []() { igm->begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, []() { igm->end(); });

	auto igmTree = CreateShared<ImGuiEntityTree>();
	igm->addPanel(igmTree);

	// Setting up the camera
	world.cameraId = em->createEntity();
	systems::ecs::updateEntityName(world.cameraId, "Main Camera");
	auto cam = em->addComponent<CameraComponent>(world.cameraId);
	cam->camera = CreateShared<ogl::Camera>();
	world.camera = systems::camera::getCamera(world.cameraId);
	world.camera->setCameraPosition(CAMERA_START_POSITION);
	em->addComponent<ColliderComponent>(world.cameraId, glm::vec3{4, 4, 4}, world.cameraSize);
	ed->post(CAMERA_UPDATE_DATA);
	enableDefaultCameraMovement();
	world.camera->updatePerspProjection(world.camera->getCameraZoom(), w->getWidth(), w->getHeight(), 0.1f, 100.f);

	ed->subscribe(CAMERA_UPDATE_DATA, [this]() {
		systems::camera::updateCameraCollider(world.cameraId, world.camera->getCameraPosition(), world.cameraSize);
	});

	ed->subscribe(CAMERA_RESET_POSITION, [this]() {
		world.camera->setCameraPosition(CAMERA_START_POSITION);
		ed->post(CAMERA_UPDATE_DATA);
	});

	// Initializing Scene
	scene->init(world.camera);

	Shared<ShaderProgram> skyboxShader = CreateShared<ShaderProgram>("skyboxVertShader.glsl", "skyboxFragShader.glsl");
	skyboxShader->createShaderProgram();
	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();
	Shared<ShaderProgram> modelShader = CreateShared<ShaderProgram>("modelVertShader.glsl", "modelFragShader.glsl");
	modelShader->createShaderProgram();
	Shared<ShaderProgram> lightShader = CreateShared<ShaderProgram>("lightVertShader.glsl", "lightFragShader.glsl");
	lightShader->createShaderProgram();

	auto skybox = factory::factorySkyBox("./resources/texture/skybox/sea/", "jpg");

	plane = factory::factoryPlane({0.3, 0.3, 0.3, 1});
	systems::ecs::updateEntityName(plane, "Basic Plane");
	scene->addEntity(shader, plane);

	auto shape = factory::factoryPyramid(BasicInfo{{1, 1, -4}, {1, 1, 1}, {}});
	scene->addEntity(lightShader, shape);
	systems::material::updateMaterial(shape, material::getMaterialFromPool(material::MATERIAL_EMERALD));
	em->addComponent<ColliderComponent>(shape);
	em->addComponent<TextureComponent>(shape, "./resources/texture/grass.png");

	TextureParams tp{};
	tp.format = GL_RGBA;
	tp.internalFormat = GL_RGBA;
	tp.border = 0;
	tp.dataType = GL_UNSIGNED_BYTE;
	tp.level = 0;
	tp.target = GL_TEXTURE_2D;

	int width, height, nC;
	auto data = readImageData("./resources/texture/grass.png", width, height, nC);
	ogl::Texture t{tp, (unsigned int)width, (unsigned int)height};
	t.onAttach();
	t.createTexture2D(data);
	t.setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	t.setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	t.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	t.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	t.generateMipmap();

	systems::texture::setTexture(shape, t);
	systems::ecs::updateEntityName(shape, "Sphere");
	auto sc = em->getComponentFromId<ShaderComponent>(shape);
	sc->computation = LightComputation::PHONG;
	sc->reflective = false;

	auto id = factory::light::factoryDirectional({1, 0, 0});

	ub = CreateUnique<UniformBuffer>("Matrices");
	ub->onAttach();
	ub->setup(sizeof(glm::mat4), 0, 0, 0);
	auto viewProj = world.camera->getViewProjMatrix();
	ub->update(0, sizeof(glm::mat4), glm::value_ptr(viewProj));

	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [this]() {
		auto vp = world.camera->getViewProjMatrix();
		ub->update(0, sizeof(glm::mat4), glm::value_ptr(vp));
	});

	ed->subscribe(ENTITY_ELECTED_CHANGED, [this, igmTree]() {
		igmTree->setSelectedEntity(ettSelected);
	});

	ed->subscribe(event::loop::LOOP_RENDER, [this, skybox, skyboxShader]() {
		// render skybox
		systems::render::renderSkybox(skybox, skyboxShader);
		// render other meshes
		systems::render::renderAllMeshes();
		if (renderBB)
			systems::render::renderBoundingBox();
		// systems::render::renderInstancedMeshes();
	});

	systems::collision::updateAllColliders();
	systems::collision::compressBoundingBox();

	auto pp = igm->addPanel<ImGuiPanel>("Profiler");
	pp->setRenderFunc([this]() {
		ImGui::Begin("Profiler");
		auto maxwall = std::distance(walltimes.begin(), std::max_element(ALL(walltimes)));
		auto maxcpu = std::distance(cputimes.begin(), std::max_element(ALL(cputimes)));
		for (auto i = 0; i < names.size(); i++) {
			ImGui::PushID(i);
			ImGui::SeparatorText(names[i].c_str());
			ImGui::TextColored((ImGui::GetIO().Framerate < 40 && maxwall == i) ? ImVec4{1, 1, 0, 1} : ImVec4{1, 1, 1, 1}, "Wall Time: %lf", walltimes[i].get());
			ImGui::TextColored((ImGui::GetIO().Framerate < 40 && maxcpu == i) ? ImVec4{1, 1, 0, 1} : ImVec4{1, 1, 1, 1}, "CPU Time: %lf", cputimes[i].get());
			ImGui::PopID();
		}
		ImGui::End();
	});

	auto cl = CreateShared<CustomLayer>();
	cl->onAttach();
	ed->subscribe(event::loop::LOOP_INPUT, [cl]() { cl->onUpdate(); });

	auto np = CreateShared<ImGuiNormalView>();
	ed->subscribe(NORMAL_VIEW_OPEN, [np, cl]() {
		igm->removePanel<ImGuiNormalView>(np);
		igm->addPanel(np);
		// disable main window movement

		cl->setRunnig(true);
		cl->setUpdateFun([&np]() {
			np->processInput();
		});

		w->setMouseButtonCallback([](auto, auto, auto, auto) {});
	});
	ed->subscribe(NORMAL_VIEW_CLOSE, [this, np, cl]() {
		igm->removePanel<ImGuiNormalView>(np);
		// enable main window movement
		defaultKeyCallback(w.get());
		changeInputState(w.get(), InputState::MOUSE_ACTIVE);
		cl->setRunnig(false);
	});
}

void NormalViewState::onDetach() {
    ASSERT(this->m_attached);
	State::onDetach();
    systems::ecs::cleanAll();
	// delete all buffers
	// delete all textures
	// delete all shaders
	igm->onDetach();
	w->onDetach();
#undef BIG
}

void NormalViewState::onUpdate() {
}

void NormalViewState::onRender() {
}

bool NormalViewState::isCurrentStateEnd() {
	return glfwWindowShouldClose(w->getContext());
}

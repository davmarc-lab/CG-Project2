#include "../../include/State/SimulationState.hpp"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include "../../include/State/BootstrapState.hpp"

#include "../../../Opengl-Core/include/Core.hpp"
#include "../../../Opengl-Core/include/Graphic.hpp"

#include "../../include/AppGui.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/EntityManager.hpp"
#include "../../include/ECS/System.hpp"
#include "../../include/Factory.hpp"

const auto em = EntityManager::instance();
const auto im = ogl::InputManager::instance();
const auto ed = ogl::EventManager::instance();
const auto scene = BasicScene::instance();
const auto sm = StateManager::instance();

const auto SPHERE_POS = glm::vec3(1, 1, -4);
const auto SPHERE_SIZE = glm::vec3(0.3f);

std::vector<glm::mat4> sphereModels{};
std::vector<glm::vec4> sphereColors{};

void SimulationState::defaultKeyCallback() {
	this->m_window->setKeysCallback([this](GLFWwindow *, int key, int, int action, int) {
		switch (action) {
			case GLFW_PRESS: {
				im->keyPressed(key);
				break;
			}
			case GLFW_RELEASE: {
				im->keyReleased(key);
				break;
			}
		}
		if (key == GLFW_KEY_C && action == GLFW_PRESS) {
			ed->post(STATE_CHANGED);
		}
	});
}

void SimulationState::enableDefaultCameraMovement() {
	ed->subscribe(event::loop::LOOP_INPUT, [this]() {
		auto collider = em->getComponentFromId<ColliderComponent>(this->m_world.cameraId);
		ASSERT(collider != nullptr);

		bool collision = false;
		if (im->isKeyPressed(GLFW_KEY_W)) {
			this->m_world.camera->moveCamera(this->m_world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(-this->m_world.camera->getCameraFront());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_S)) {
			this->m_world.camera->moveCamera(-this->m_world.camera->getCameraFront());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(this->m_world.camera->getCameraFront());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_D)) {
			this->m_world.camera->moveCamera(this->m_world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(-this->m_world.camera->getCameraRight());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_A)) {
			this->m_world.camera->moveCamera(-this->m_world.camera->getCameraRight());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(this->m_world.camera->getCameraRight());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_SPACE)) {
			this->m_world.camera->moveCamera(this->m_world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(-this->m_world.camera->getCameraUp());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
		if (im->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			this->m_world.camera->moveCamera(-this->m_world.camera->getCameraUp());
			ed->post(CAMERA_UPDATE_DATA);
			for (auto [first, second] : systems::collision::getCollisions()) {
				if (first == this->m_world.cameraId || second == this->m_world.cameraId) {
					this->m_world.camera->moveCamera(this->m_world.camera->getCameraUp());
					ed->post(CAMERA_UPDATE_DATA);
				}
			}
		}
	});
}

float randf() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 getRandVec3() {
	return glm::vec3(randf(), randf(), randf());
}

glm::vec3 getPosNear(const glm::vec3 &pos) {
	return (getRandVec3() / glm::vec3{2}) + (rand() % 2 == 0 ? pos : -pos);
}

void SimulationState::onAttach() {
	ASSERT(!this->m_attached);
	State::onAttach();
	srand(time(NULL));
	ogl::WindowSettings settings{};
	settings.decorated = false;
	settings.size = {1366, 768};
	settings.position = {400, 12};
	settings.bgColor = {.3, .3, .3, 1};

	this->m_window = CreateUnique<ogl::Window>(settings);
	this->m_window->onAttach();

	this->defaultKeyCallback();

	glEnable(GL_CULL_FACE);
	ed->subscribe(ogl::event::loop::LOOP_UPDATE, [this]() { this->m_window->onUpdate(); });
	ed->subscribe(ogl::event::loop::LOOP_RENDER, [this]() { this->m_window->onRender(); });

	ogl::Renderer::instance()->init();

	this->m_img = CreateUnique<ogl::ImGuiManager>(this->m_window.get(), ogl::DEFAULT_IMGUI_CONFIGS);
	this->m_img->onAttach();

	ed->subscribe(ogl::event::loop::LOOP_UPDATE, [this]() { this->m_img->onUpdate(); });
	ed->subscribe(ogl::event::loop::LOOP_RENDER, [this]() { this->m_img->onRender(); });
	ed->subscribe(ogl::event::loop::LOOP_BEGIN_RENDER, [this]() { this->m_img->begin(); });
	ed->subscribe(ogl::event::loop::LOOP_END_RENDER, [this]() { this->m_img->end(); });

	// Physic world
	this->m_pw.onAttach();
	this->m_pw.addSolver<CollisionSolver>();
	this->m_pw.addSolver<PositionSolver>();

	this->m_world.cameraId = em->createEntity();
	systems::ecs::updateEntityName(this->m_world.cameraId, "World Camera");
	auto cam = em->addComponent<CameraComponent>(this->m_world.cameraId);
	cam->camera = CreateShared<ogl::Camera>();
	this->m_world.camera = systems::camera::getCamera(this->m_world.cameraId);
	this->m_world.camera->setCameraPosition(CAMERA_START_POSITION);
	em->addComponent<ColliderComponent>(this->m_world.cameraId, glm::vec3{4, 4, 4}, this->m_world.cameraSize);
	enableDefaultCameraMovement();
	this->m_world.camera->updatePerspProjection(this->m_world.camera->getCameraZoom(), this->m_window->getWidth(), this->m_window->getHeight(), 0.1f, 100.f);

	ed->subscribe(CAMERA_UPDATE_DATA, [this]() {
		systems::camera::updateCameraCollider(this->m_world.cameraId, this->m_world.camera->getCameraPosition(), this->m_world.cameraSize);
	});

	ed->subscribe(CAMERA_RESET_POSITION, [this]() {
		this->m_world.camera->setCameraPosition(CAMERA_START_POSITION);
		ed->post(CAMERA_UPDATE_DATA);
	});

	scene->init(this->m_world.camera);

	this->m_ub = CreateUnique<UniformBuffer>("Matrices");
	this->m_ub->onAttach();
	this->m_ub->setup(sizeof(glm::mat4), 0, 0, 0);
	auto viewProj = this->m_world.camera->getViewProjMatrix();
	m_ub->update(0, sizeof(glm::mat4), glm::value_ptr(viewProj));

	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [this]() {
		auto vp = this->m_world.camera->getViewProjMatrix();
		this->m_ub->update(0, sizeof(glm::mat4), glm::value_ptr(vp));
	});

	Shared<ShaderProgram> skyboxShader = CreateShared<ShaderProgram>("skyboxVertShader.glsl", "skyboxFragShader.glsl");
	skyboxShader->createShaderProgram();
	Shared<ShaderProgram> planeShader = CreateShared<ShaderProgram>("vertexShader.glsl", "basicFS.glsl");
	planeShader->createShaderProgram();

	auto skybox = factory::factorySkyBox("./resources/texture/skybox/lycksele/", "jpg");

	auto plane = factory::factoryPlane({.3f, .3f, .3f, 1});
	scene->addEntity(planeShader, plane);
	this->m_pw.addEntity(plane);

	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::collision::updateAllColliders(); });

	SimulationConfig config{};
	config.gravity = {0, -9.81f, 0};
	// simulation panel
	auto sp = this->m_img->addPanel<ImGuiSimulationPanel>(config);
	this->m_img->addPanel<ImGuiEntityTree>();

	ed->subscribe(RUN_SIMULATION, [this, sp]() {
		sp->setRunning(true);
		this->m_pw.onAttach();
		this->m_pw.addSolver<CollisionSolver>();
		this->m_pw.addSolver<PositionSolver>();
	});
	ed->subscribe(STOP_SIMULATION, [this, sp]() {
		sp->setRunning(false);
		this->m_pw.onDetach();
	});

	ed->subscribe(event::loop::LOOP_UPDATE, [this, sp]() {
		if (sp->isRunning()) {
			this->m_pw.onUpdate();
		} else {
			this->m_pw.resetDeltaTime();
		}
	});

	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, []() {
		sphereModels.clear();
		for (auto e : em->getEntitiesFromComponent<InstancedComponent>()) {
			sphereModels.push_back(systems::transform::getModelMatrix(e));
		}

		systems::render::prepareInstancedMesh(sphereModels, sphereColors);
	});

	ed->subscribe(event::loop::LOOP_RENDER, [skybox, skyboxShader]() {
		systems::render::renderSkybox(skybox, skyboxShader);
		systems::render::renderAllMeshes();
		systems::render::renderInstancedMeshes();
	});

	ed->subscribe(STATE_CHANGED, []() {
		sm->changeState(BOOTSTRAP_STATE_NAME);
	});

	ed->subscribe(ADD_SPHERE, [this]() {
		auto color = glm::vec4{getRandVec3(), 1};
		sphereColors.push_back(color);
		auto shape = factory::factorySphereInstanced(BasicInfo{getPosNear(SPHERE_POS), SPHERE_SIZE, {}}, color);
		systems::collision::updateColliderType(shape, ColliderType::COLLIDER_SPHERE);
		systems::material::updateMaterial(shape, material::getMaterialFromPool(material::MATERIAL_NONE));

		auto sc = em->getComponentFromId<ShaderComponent>(shape);
		sc->computation = LightComputation::NONE;
		sc->reflective = false;
		em->addComponent<PhysicComponent>(shape);
		sphereModels.push_back(systems::transform::getModelMatrix(shape));

		this->m_pw.addEntity(shape);
	});
}

void SimulationState::onDetach() {
	ASSERT(this->m_attached);
	State::onDetach();
	systems::ecs::cleanAll();
	// delete all buffers
	// delete all textures
	// delete all shaders
	this->m_img->onDetach();
	this->m_window->onDetach();
}

void SimulationState::onUpdate() {}

void SimulationState::onRender() {}

bool SimulationState::isCurrentStateEnd() {
	return glfwWindowShouldClose(this->m_window->getContext());
}

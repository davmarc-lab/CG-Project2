#include "../../include/State/SimulationState.hpp"
#include <glm/gtc/type_ptr.hpp>

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
	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();

	auto skybox = factory::factorySkyBox("./resources/texture/skybox/sea/", "jpg");

	auto shape = factory::factorySphere(BasicInfo{{1, 1, -4}, {1, 1, 1}, {}});
	scene->addEntity(shader, shape);
	em->addComponent<ColliderComponent>(shape);
	systems::material::updateMaterial(shape, material::getMaterialFromPool(material::MATERIAL_EMERALD));

	systems::ecs::updateEntityName(shape, "Sphere");
	auto sc = em->getComponentFromId<ShaderComponent>(shape);
	sc->computation = LightComputation::PHONG;
	sc->reflective = false;

	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::collision::updateAllColliders(); });

	ed->subscribe(event::loop::LOOP_RENDER, [skybox, skyboxShader]() {
		systems::render::renderSkybox(skybox, skyboxShader);
		systems::render::renderAllMeshes();
	});
}

void SimulationState::onDetach() {}

void SimulationState::onUpdate() {}

void SimulationState::onRender() {}

bool SimulationState::isCurrentStateEnd() {
	return glfwWindowShouldClose(this->m_window->getContext());
}

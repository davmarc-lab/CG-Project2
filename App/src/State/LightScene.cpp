#include "../../include/State/LightScene.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "../../../Opengl-Core/include/Core.hpp"
#include "../../../Opengl-Core/include/Graphic.hpp"

#include "../../include/AppGui.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/EntityManager.hpp"
#include "../../include/ECS/System.hpp"
#include "../../include/Factory.hpp"

#include "../../include/PBR/PBScene.hpp"

#include "../../include/Profiler.hpp"

const auto em = EntityManager::instance();
const auto im = ogl::InputManager::instance();
const auto ed = ogl::EventManager::instance();
const auto scene = BasicScene::instance();
const auto pbscene = PBScene::instance();
const auto sm = StateManager::instance();

const auto pf = LinuxProfiler::instance();

Mouse mouse{};

void LightState::onAttach() {
	ASSERT(!this->m_attached);
	State::onAttach();
	srand(time(NULL));
	ogl::WindowSettings settings{};
	settings.decorated = false;
	settings.size = {800, 600};
	settings.position = {400, 50};
	settings.bgColor = {.3, .3, .3, 1};

	this->m_window = CreateUnique<ogl::Window>(settings);
	this->m_window->onAttach();

	defaultKeyCallback(this->m_window.get(), this->m_world, mouse);

	glEnable(GL_CULL_FACE);
	ed->subscribe(ogl::event::loop::LOOP_UPDATE, [this]() { this->m_window->onUpdate(); });
	ed->subscribe(ogl::event::loop::LOOP_RENDER, [this]() { this->m_window->onRender(); });

	ogl::Renderer::instance()->init();

	this->m_igm = CreateUnique<ogl::ImGuiManager>(this->m_window.get(), ogl::DEFAULT_IMGUI_CONFIGS);
	this->m_igm->onAttach();

	ed->subscribe(ogl::event::loop::LOOP_UPDATE, [this]() { this->m_igm->onUpdate(); });
	ed->subscribe(ogl::event::loop::LOOP_RENDER, [this]() { this->m_igm->onRender(); });
	ed->subscribe(ogl::event::loop::LOOP_BEGIN_RENDER, [this]() { this->m_igm->begin(); });
	ed->subscribe(ogl::event::loop::LOOP_END_RENDER, [this]() { this->m_igm->end(); });

	this->m_world.cameraId = em->createEntity();
	systems::ecs::updateEntityName(this->m_world.cameraId, "Main Camera");
	auto cam = em->addComponent<CameraComponent>(this->m_world.cameraId);
	cam->camera = CreateShared<ogl::Camera>();
	this->m_world.camera = systems::camera::getCamera(this->m_world.cameraId);
	this->m_world.camera->setCameraPosition(CAMERA_START_POSITION);
	em->addComponent<ColliderComponent>(this->m_world.cameraId, glm::vec3{4, 4, 4}, this->m_world.cameraSize);
	ed->post(CAMERA_UPDATE_DATA);

	defaultCameraMovement(this->m_world);

	this->m_world.camera->updatePerspProjection(this->m_world.camera->getCameraZoom(), this->m_window->getWidth(), this->m_window->getHeight(), 0.1f, 100.f);

	ed->subscribe(CAMERA_UPDATE_DATA, [this]() {
		systems::camera::updateCameraCollider(this->m_world.cameraId, this->m_world.camera->getCameraPosition(), this->m_world.cameraSize);
	});

	ed->subscribe(CAMERA_RESET_POSITION, [this]() {
		this->m_world.camera->setCameraPosition(CAMERA_START_POSITION);
		ed->post(CAMERA_UPDATE_DATA);
	});

	scene->init(this->m_world.camera);

	this->m_ub = CreateUnique<UniformBuffer>();
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
	Shared<ShaderProgram> modelShader = CreateShared<ShaderProgram>("modelVertShader.glsl", "modelFragShader.glsl");
	modelShader->createShaderProgram();
	Shared<ShaderProgram> pbrShader = CreateShared<ShaderProgram>("pbrVertShader.glsl", "pbrFragShader.glsl");
	pbrShader->createShaderProgram();

	auto skybox = factory::factorySkyBox("./resources/texture/skybox/lycksele/", "jpg");

	auto plane = factory::factoryPlane({.3f, .3f, .3f, 1});
	systems::ecs::updateEntityName(plane, "Plane");
	scene->addEntity(planeShader, plane);

	// entities
	auto foo = factory::factorySphere(BasicInfo{{-2, 0, -4}, {1, 1, 1}, {0, 0, 0}});
	em->removeComponent<MaterialComponent>(foo);
	em->addComponent<PBMaterial>(foo);
	systems::pbr::updateMaterial(foo, pbr::metal);
	systems::light::setLightComputation(foo, LightComputation::PHONG);
	pbscene->addEntity(foo);

	{
		// Too heavy
		auto model = factory::factoryObjMesh(BasicInfo{{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}, "resources/models/machine/Machine.obj");
		scene->addEntity(modelShader, model);
	}

	{
		// Too heavy
		flipImagesVertically(true);
		auto model = factory::factoryObjMesh(BasicInfo{{0, 0, 0}, {1, 1, 1}, {0, 0, 0}}, "resources/models/backpack/backpack.obj");
		flipImagesVertically(false);
		scene->addEntity(modelShader, model);
	}

	// lights
	// auto dir = factory::light::factoryDirectional({1, 1, -1});
	auto dir = factory::light::factoryPoint({0, 0, -1}, {});
	auto aaa = factory::light::factoryPoint({-3, 0, -2}, {});
	auto bbb = factory::light::factoryPoint({0, 2, -3}, {});

	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::collision::updateAllColliders(); });

	this->m_igm->addPanel<ImGuiEntityTree>();
	auto pp = this->m_igm->addPanel<ImGuiPanel>("Profiler");
	pp->setRenderFunc([this]() {
		ImGui::Begin("Profiler");

		auto input = pf->getInputTime();
		auto update = pf->getUpdateTime();
		auto render = pf->getRenderTime();

		ImGui::SeparatorText("Input");
		ImGui::Text("Wall time: %lf", input.x);
		ImGui::Text("Cpu time: %lf", input.y);
		ImGui::SeparatorText("Update");
		ImGui::Text("Wall time: %lf", update.x);
		ImGui::Text("Cpu time: %lf", update.y);
		ImGui::SeparatorText("Render");
		ImGui::Text("Wall time: %lf", render.x);
		ImGui::Text("Cpu time: %lf", render.y);

		ImGui::End();
	});

	ed->subscribe(event::loop::LOOP_RENDER, [this, skybox, skyboxShader, pbrShader]() {
		systems::render::renderSkybox(skybox, skyboxShader);
		systems::render::renderAllMeshes();
		systems::render::renderScene(pbrShader, this->m_world);
	});

	ed->subscribe(STATE_CHANGED, []() {
		// sm->changeState(BOOTSTRAP_STATE_NAME);
	});
}
void LightState::onDetach() {
	ASSERT(this->m_attached);
	State::onDetach();

	this->m_igm->onDetach();
	this->m_window->onDetach();
}

void LightState::onUpdate() {}
void LightState::onRender() {}

bool LightState::isCurrentStateEnd() {
	return glfwWindowShouldClose(this->m_window->getContext());
}

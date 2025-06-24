#include <utility>
#include "../../Opengl-Core/include/Core.hpp"

#include "../include/ECS/EcsScene.hpp"
#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../include/AppGui.hpp"

#include "../include/Factory.hpp"
#include "../include/PhysicWorld.hpp"
#include "../include/Profiler.hpp"
#include "../include/State/DefaultState.hpp"
#include "../include/State/State.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <functional>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <iterator>
#include <memory>
#include <vector>

using namespace ogl;

#define BIG

const auto ed = EventManager::instance();
const auto im = InputManager::instance();
const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

// double inputWalltime{}, inputCputime{};
// double updateWalltime{}, updateCputime{};
// double renderWalltime{}, renderCputime{};
// std::array<std::reference_wrapper<double>, 3> walltimes{inputWalltime, updateWalltime, renderWalltime};
// std::array<std::reference_wrapper<double>, 3> cputimes{inputCputime, updateCputime, renderCputime};
// std::array<std::string, 3> names{"Input", "Update", "Render"};

std::vector<glm::mat4> sphereModels{};
std::vector<glm::vec4> sphereColors{};

#ifdef _WIN32
WProfiler profiler{PROFILE_ALL};
#else
LinuxProfiler profiler{PROFILE_ALL};
#endif

float randf() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 getRandColor() {
	return glm::vec3(randf(), randf(), randf());
}

glm::vec3 getRandVelocity(float time = 1) {
	return std::sin(time) * glm::vec3(randf() + std::rand() % 5 - 2, randf() + std::rand() % 5 - 2, randf() + std::rand() % 5 - 2);
}

glm::vec3 evaluateNormal(const unsigned int &id) {
	auto v = em->getComponentFromId<VertexComponent>(id);
	glm::vec3 norm{};
	glm::vec3 v1 = glm::vec4(v->getVertexCoords()[v->getIndexCoords()[0]], 0) * systems::transform::getModelMatrix(id);
	glm::vec3 v2 = glm::vec4(v->getVertexCoords()[v->getIndexCoords()[1]], 0) * systems::transform::getModelMatrix(id);
	glm::vec3 v3 = glm::vec4(v->getVertexCoords()[v->getIndexCoords()[2]], 0) * systems::transform::getModelMatrix(id);

	v1 -= v2;
	v3 -= v2;
	norm = glm::normalize(glm::cross(v1, v3));

	return norm;
}

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

const auto sm = StateManager::instance();

int main(int argc, char *argv[]) {
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, []() {
		sphereModels.clear();
		for (auto e : em->getEntitiesFromComponent<InstancedComponent>()) {
			sphereModels.push_back(systems::transform::getModelMatrix(e));
		}
		ogl::Renderer::instance()->prepareBuffers(sphereModels, sphereColors);
	});

	auto ds = CreateShared<DefaultState>();

	std::cout << "Attaching \"" << ds->getName() << "\" to State Manager\n";
	sm->changeState(ds->getName(), ds);

	ed->subscribe(event::loop::LOOP_UPDATE, []() { sm->execUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, []() { sm->execRender(); });

	// #define C_DBG

	while (!ds->isCurrentStateEnd()) {
#ifdef C_DBG
		std::cout << "\n---START---\n";
#endif
		ed->post(event::loop::LOOP_INPUT);
#ifdef C_DBG
		std::cout << "INP\n";
#endif
		ed->post(event::loop::LOOP_UPDATE);
#ifdef C_DBG
		std::cout << "UPD\n";
#endif
		ed->post(event::loop::LOOP_BEGIN_RENDER);
#ifdef C_DBG
		std::cout << "BR\n";
#endif
		ed->post(event::loop::LOOP_RENDER);
#ifdef C_DBG
		std::cout << "R\n";
#endif
		ed->post(event::loop::LOOP_END_RENDER);
#ifdef C_DBG
		std::cout << "ER\n";
		std::cout << "---END---\n";
#endif
	}

	// while (!glfwWindowShouldClose(w.getContext())) {
	// 	profiler.start();
	// 	ed->post(event::loop::LOOP_INPUT);
	// 	profiler.end();
	// 	profiler.dump(inputWalltime, inputCputime);
	//
	// 	profiler.start();
	// 	ed->post(event::loop::LOOP_UPDATE);
	// 	profiler.end();
	// 	profiler.dump(updateWalltime, updateCputime);
	//
	// 	ed->post(event::loop::LOOP_BEGIN_RENDER);
	// 	profiler.start();
	// 	ed->post(event::loop::LOOP_RENDER);
	// 	profiler.end();
	// 	profiler.dump(renderWalltime, renderCputime);
	// 	ed->post(event::loop::LOOP_END_RENDER);
	// }

	// detach State Manager
	sm->clean();

	// igm.onDetach();
	// w.onDetach();

	return 0;
}

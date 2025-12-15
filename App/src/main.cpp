#include "../../Opengl-Core/include/Core.hpp"

#include "../include/ECS/EcsScene.hpp"
#include "../include/ECS/EntityManager.hpp"

#include "../include/Profiler.hpp"
#include "../include/State/BootstrapState.hpp"
#include "../include/State/LightScene.hpp"
#include "../include/State/NormalViewState.hpp"
#include "../include/State/SimulationState.hpp"
#include "../include/State/State.hpp"

#include <GLFW/glfw3.h>

#include <functional>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ogl;

#define BIG

const auto ed = EventManager::instance();
const auto im = InputManager::instance();
const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();
const auto sm = StateManager::instance();

#ifdef _WIN32
WProfiler profiler{PROFILE_ALL};
#else
auto profiler = LinuxProfiler::instance();
#endif

int main(int argc, char *argv[]) {
	auto st = CreateShared<SimulationState>();
	sm->cacheState(st);

	auto ls = CreateShared<LightState>();
	sm->cacheState(ls);

	auto ds = CreateShared<NormalViewState>();
	sm->cacheState(ds);

	auto bs = CreateShared<BootstrapState>();
	sm->cacheState(bs);

	sm->changeState(bs);
	sm->sync();

	// #define C_DBG

	while (!sm->shouldExit()) {
#ifdef C_DBG
		std::cout << "\n---START---\n";
#endif

		profiler->start();
		ed->post(event::loop::LOOP_INPUT);
		profiler->end();
		profiler->dumpInput();
#ifdef C_DBG
		std::cout << "INP\n";
#endif
		profiler->start();
		ed->post(event::loop::LOOP_UPDATE);
		profiler->end();
		profiler->dumpUpdate();
#ifdef C_DBG
		std::cout << "UPD\n";
#endif
		profiler->start();
		ed->post(event::loop::LOOP_BEGIN_RENDER);
#ifdef C_DBG
		std::cout << "BR\n";
#endif
		ed->post(event::loop::LOOP_RENDER);
#ifdef C_DBG
		std::cout << "R\n";
#endif
		ed->post(event::loop::LOOP_END_RENDER);
		profiler->end();
		profiler->dumpRender();
#ifdef C_DBG
		std::cout << "ER\n";
		std::cout << "---END---\n";
#endif
		sm->sync();
	}

	// detach State Manager
	sm->clean();

	return 0;
}

#include "../../Opengl-Core/include/Core.hpp"

#include "../include/Profiler.hpp"
#include "../include/State/BootstrapState.hpp"
#include "../include/State/LightScene.hpp"
#include "../include/State/LoaderState.hpp"
#include "../include/State/NormalViewState.hpp"
#include "../include/State/SimulationState.hpp"
#include "../include/State/State.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>

using namespace ogl;

#define BIG

const auto ed = EventManager::instance();
const auto sm = StateManager::instance();

#ifdef _WIN32
WProfiler profiler{PROFILE_ALL};
#else
auto profiler = LinuxProfiler::instance();
#endif

void old() {
	// instance all the states
	auto st = CreateShared<SimulationState>();
	// caching teh state avoid instancing every time it changes
	sm->cacheState(st);

	auto ls = CreateShared<LightState>();
	sm->cacheState(ls);

	auto mls = CreateShared<LoaderState>();
	sm->cacheState(mls);

	auto ds = CreateShared<NormalViewState>();
	sm->cacheState(ds);

	auto bs = CreateShared<BootstrapState>();
	sm->cacheState(bs);

	// change and sync the current state
	sm->changeState(bs);
	sm->sync();

	// #define C_DBG

	// simple game loop
	// all the ifdef are used only for debugging
	while (!sm->shouldExit()) {
#ifdef C_DBG
		std::cout << "\n---START---\n";
#endif

		// start measuring time
		profiler->start();
		ed->post(event::loop::LOOP_INPUT);
		// stop measuring time
		profiler->end();
		// save elapsed time off input step
		profiler->dumpInput();
#ifdef C_DBG
		std::cout << "INP\n";
#endif
		// update step time
		profiler->start();
		ed->post(event::loop::LOOP_UPDATE);
		profiler->end();
		profiler->dumpUpdate();
#ifdef C_DBG
		std::cout << "UPD\n";
#endif
		// render step time ( pre render + render + after render )
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
}

#include <json/json.h>

int main(int argc, char *argv[]) {
	const std::string file = "./resources/mesh/file.json";
	std::ifstream f(file);

	Json::Value root;
	Json::CharReaderBuilder builder;
	std::string errs;
	Json::parseFromStream(builder, f, &root, &errs);
	f.close();
	if (!errs.empty())
		return 1;

	JsonSerializer s{};

	auto &etts = root["entities"];
	std::for_each(ALL(etts), [](auto &e) { e["name"] = "A"; });
	std::cout << etts << "\n";

	s.serializeToFile(root, file);

	return EXIT_SUCCESS;
}

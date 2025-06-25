#include "../../include/State/BootstrapState.hpp"

#include "../../../Opengl-Core/include/Core.hpp"
#include "../../../Opengl-Core/include/Graphic.hpp"
using namespace ogl;

#include "../../include/AppGui.hpp"

const auto ed = EventManager::instance();

void BootstrapState::onAttach() {
	State::onAttach();
	WindowSettings s{};
	s.bgColor = {1, 1, 1, 1};
	s.decorated = false;
	s.size = {800, 600};
	s.position = {400, 400};
	this->window = CreateUnique<Window>(s);
	this->window->onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [this]() { this->window->onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [this]() { this->window->onRender(); });

	this->igm = CreateUnique<ImGuiManager>("ImGui Manager", this->window.get(), DEFAULT_IMGUI_CONFIGS | ImGuiConfigFlags_ViewportsEnable);
	this->igm->onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [this]() { this->igm->onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [this]() { this->igm->onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [this]() { this->igm->begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [this]() { this->igm->end(); });

	this->igm->addPanel<ImGuiEntityTree>();
}

void BootstrapState::onDetach() {
	State::onDetach();
	this->igm->onDetach();
	this->window->onDetach();
}

void BootstrapState::onUpdate() {}

void BootstrapState::onRender() {}
bool BootstrapState::isCurrentStateEnd() {
	return glfwWindowShouldClose(this->window->getContext());
}

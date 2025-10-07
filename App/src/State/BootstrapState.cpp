#include "../../include/State/BootstrapState.hpp"

#include "../../../Opengl-Core/include/Core.hpp"
#include "../../../Opengl-Core/include/Graphic.hpp"
using namespace ogl;

#include "../../include/AppGui.hpp"

const auto ed = EventManager::instance();
const auto sm = StateManager::instance();

void BootstrapState::onAttach() {
    ASSERT(!this->m_attached);
	State::onAttach();
	WindowSettings s{};
	s.bgColor = {1, 1, 1, 1};
	s.decorated = false;
	s.size = {400, 300};
	s.position = {200, 50};
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
}

void BootstrapState::onDetach() {
	State::onDetach();
	this->igm->onDetach();
	this->window->onDetach();
}

ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
bool open = true;
void BootstrapState::onUpdate() {
}

ImGuiWindowFlags windowFlags = 0;
void BootstrapState::onRender() {
	// ImGui::DockSpaceOverViewport();

	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin("DockSpace Demo", &open, windowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);
	{
		ImGuiIO &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
			// ImGui::SetNextWindowDockID(dockspace_id);
		}
		ImGui::Begin("States");
		ImGui::BeginTable("States Table", 2);
		for (auto elem : sm->getCachedStates()) {
			auto name = elem.first;
			auto state = elem.second;
			ImGui::PushID(name.c_str());
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", name.c_str());
			ImGui::TableNextColumn();
			if (ImGui::Button("Open")) {
				sm->changeState(state);
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
		ImGui::End();
	}
	ImGui::End();
}
bool BootstrapState::isCurrentStateEnd() {
	return glfwWindowShouldClose(this->window->getContext());
}

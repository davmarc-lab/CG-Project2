#pragma once

#include "State.hpp"

const std::string BOOTSTRAP_STATE_NAME = "Bootstrap";

/**
 * @brief This State allows to load one State in an ImGui window from 
 * cached states in the StateManager.
 */
class BootstrapState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

    /**
    * @brief Instances basic state with the given name.
    *
    * @param name state name
    */
	BootstrapState() :
		State(BOOTSTRAP_STATE_NAME) {}

	virtual ~BootstrapState() override = default;

private:
	/// pointer to the window
	Unique<ogl::Window> window;
	/// pointer to the imgui manager
	Unique<ogl::ImGuiManager> igm;
};

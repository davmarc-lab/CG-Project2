#pragma once

#include "State.hpp"

class BootstrapState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

	BootstrapState() :
		State("Bootstrap State") {}

	virtual ~BootstrapState() override = default;

private:
	Unique<ogl::Window> window;
	Unique<ogl::ImGuiManager> igm;
};

#pragma once

#include "../../include/State/State.hpp"

class DefaultState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

    virtual bool isCurrentStateEnd() override;

	DefaultState() :
		State("Default State") {}

	virtual ~DefaultState() override = default;

private:
};

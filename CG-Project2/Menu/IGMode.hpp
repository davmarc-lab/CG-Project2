#pragma once

#include "IGMenu.hpp"

#include "../Lib.hpp"
#include "../LibCustomGui.hpp"

class IGMode : public IGMenu {
private:
	InputMode* mode;
public:

	IGMode(InputMode* mode) : mode(mode) {}

	virtual void render() {
		string text = calcInputMode(*this->mode);

		ImGui::Begin("Input Mode");
		ImGui::Text(text.c_str());
		ImGui::End();
	}

	~IGMode() = default;

};
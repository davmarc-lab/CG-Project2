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
        ImGui::TextColored(ImVec4(0.92f, 0.78f, 0.2f, 1.f), "Press `v` for Select mode \
            \nPress `g` for Interact mode \
            \nPress `p` for Passive mode \
            \nPress `q` to Quit");
		ImGui::Text("%s", text.c_str());
		ImGui::End();
	}

	~IGMode() = default;

};

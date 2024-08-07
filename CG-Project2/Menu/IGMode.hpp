#pragma once


#include "IGMenu.hpp"

#include "../Action/ActionManager.hpp"

#include "../Lib.hpp"

/*
 * This class creates a simple tracker to see the current input mode.
 */
class IGMode : public IGMenu {
private:
	InputMode* mode;
public:

	IGMode(InputMode* mode) : mode(mode) {}

	inline virtual void render() override {
		string text = calcInputMode(*this->mode);

		ImGui::Begin("Input Mode");
        ImGui::TextColored(ImVec4(0.92f, 0.78f, 0.2f, 1.f), "Press `v` for Select mode \
            \nPress `g` for Interact mode \
            \nPress `p` for Passive mode \
            \nPress `q` to Quit");
		ImGui::Text("%s", text.c_str());
        ImGui::Separator();
        // This button enables the render in another custom FrameBuffer, implemented for creating animations (not done)
        if (ImGui::Button("Play")) {
            ActionManager::instance()->addAction(Action::START_SIM);
        }
        ImGui::SameLine();
        // This button closes the animation panel and restore the default FrameBuffer
        if (ImGui::Button("Stop")) {
            ActionManager::instance()->addAction(Action::STOP_SIM);
        }
		ImGui::End();
	}

	~IGMode() = default;

};

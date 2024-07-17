#pragma once

#include "IGMenu.hpp"

#include "Logger/Log.hpp"

#include <iostream>

using namespace std;

class IGDebug : private IGMenu {
  private:
    std::vector<std::pair<LogType, std::string>> logs;

    bool track_logs = true;
    int track_item = -1;

    static IGDebug *pointer;

  protected:
    IGDebug() {}

  public:
    IGDebug(const IGDebug &obj) = delete;

    inline static IGDebug *instance() {
        if (pointer == nullptr) {
            pointer = new IGDebug();
        }

        return pointer;
    }

    inline void addLog(LogType type, std::string text) {
        this->track_item += 1;
        this->logs.push_back(std::pair(type, text));
    }

    inline virtual void render() override {
        ImGui::Begin("Debug Logger");
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
            ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1),
                                                ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * 10));
            if (ImGui::BeginChild("Log Messages", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Border)) {
                ImGui::PopStyleColor();
                for (auto elem : this->logs) {
                    ImGui::PushID(&elem);
                    ImGui::TextColored(getLogColor(elem.first), "[%s]", getLogText(elem.first).c_str());
                    ImGui::SameLine();
                    ImGui::Text("=> %s", elem.second.c_str());
                    if (this->track_logs && this->track_item == this->logs.size() - 1) {
                        ImGui::SetScrollHereY();
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();
            ImGui::Checkbox("Track", &this->track_logs);
        }
        ImGui::End();
    }
};

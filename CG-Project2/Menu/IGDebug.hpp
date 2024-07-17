#pragma once

#include "IGMenu.hpp"

#include "Logger/Log.hpp"

#include "../Lib.hpp"

#include <set>

#define NUM_LOG_TYPES 6

class IGDebug : private IGMenu {
  private:
    std::vector<std::pair<LogType, std::string>> logs;

    LogType log_types[NUM_LOG_TYPES] = {LogType::GENERAL_EVENT, LogType::MISSING_IMPLEMENTATION, LogType::MISSING_CASTER, LogType::SELECT_WARNING, LogType::ERROR, LogType::EMPTY};
    std::set<LogType> active_log_filter;

    bool track_logs = true;
    int track_item = -1;

    static IGDebug *pointer;

    inline void resetTrackIndex() { this->track_item = -1; }

  protected:
    IGDebug() {
        // default log filter
        this->active_log_filter.insert(GENERAL_EVENT);
    }

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
            ImGui::SeparatorText("Log Type Filter");
            ImGui::BeginGroup();
            for (auto t : this->log_types) {
                ImGui::PushID(t);
                if (this->active_log_filter.find(t) != this->active_log_filter.end()) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0, 0, 1));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                }
                if (ImGui::Button(getLogLongText(t).c_str())) {
                    if (this->active_log_filter.find(t) != this->active_log_filter.end())
                        this->active_log_filter.erase(t);
                    else
                        this->active_log_filter.insert(t);
                }
                ImGui::PopStyleColor(1);
                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::EndGroup();
        }

        {
            if (ImGui::Button("Reset")) {
                this->active_log_filter.clear();
            }
            ImGui::SameLine();
            if (ImGui::Button("All")) {
                if (this->active_log_filter.size() != NUM_LOG_TYPES)
                    for (auto l : this->log_types) {
                        this->active_log_filter.insert(l);
                    }
                else
                    this->active_log_filter.clear();
            }
        }

        ImGui::SeparatorText("Logs");
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
            ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1),
                                                ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * 10));
            if (ImGui::BeginChild("Log Messages", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Border)) {
                ImGui::PopStyleColor();
                for (auto elem : this->logs) {
                    if (this->active_log_filter.find(elem.first) != this->active_log_filter.end()) {
                        ImGui::PushID(&elem);
                        ImGui::TextColored(getLogColor(elem.first), "(TIME) [%s]", getLogText(elem.first).c_str());
                        ImGui::SameLine();
                        ImGui::Text("=> %s", elem.second.c_str());
                        ImGui::PopID();
                    }
                    if (this->track_logs && this->track_item == this->logs.size() - 1) {
                        ImGui::SetScrollHereY();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::Checkbox("Track", &this->track_logs);
        }
        if (ImGui::Button("Clear##1")) {
            this->logs.clear();
            resetTrackIndex();
        }
        ImGui::End();
    }
};

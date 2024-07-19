#pragma once

#include "IGMenu.hpp"

#include "Logger/Log.hpp"

#include "Logger/LogManager.hpp"

#include <set>

class IGDebug : private IGMenu {
  private:
    LogManager *log_manager = LogManager::instance();

    std::set<LogType> active_log_filter;

    bool track_logs = true;

  public:

    IGDebug() {
        // default log filter
        this->active_log_filter.insert(logs::GENERAL_EVENT);
    }

    inline void addLog(Log log) {
        log_manager->addLog(log);
    }

    inline void addLog(LogType log, float time, const char* text) { this->addLog(Log(log, time, text)); }

    inline void addLog(const char *log_type, const char *log_text, ImVec4 color, float time, const char* text) { this->addLog(Log(log_type, log_text, color, time, text)); }

    inline virtual void render() override {
        ImGui::Begin("Debug Logger");
        {
            ImGui::SeparatorText("Log Type Filter");
            ImGui::BeginGroup();
            for (auto t : logs::log_types) {
                ImGui::PushID(&t);
                if (this->active_log_filter.find(t) != this->active_log_filter.end()) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0, 0, 1));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                }
                if (ImGui::Button(t.getLogTypeText())) {
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
                if (this->active_log_filter.size() != logs::log_types.size())
                    for (auto l : logs::log_types) {
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
            if (ImGui::BeginChild("Log Messages", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                                  ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {
                ImGui::PopStyleColor();
                for (auto elem : log_manager->getLogs()) {
                    if (this->active_log_filter.find(elem.getLogTypeObject()) != this->active_log_filter.end()) {
                        ImGui::PushID(&elem);
                        ImGui::TextColored(elem.getLogTypeColor(), "(%f) [%s]", elem.getLogTime(), elem.getLogType());
                        ImGui::SameLine();
                        ImGui::Text("=> %s", elem.getLogText());
                        ImGui::PopID();
                    }
                    if (this->track_logs && log_manager->getTrack() == log_manager->getLogs().size() - 1) {
                        ImGui::SetScrollHereY();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::Checkbox("Track", &this->track_logs);
        }
        if (ImGui::Button("Clear##1")) {
            log_manager->clear();
            log_manager->resetTrack();
        }
        ImGui::End();
    }
};

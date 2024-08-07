#pragma once

#include "IGMenu.hpp"

#include "Logger/Log.hpp"

#include "Logger/LogManager.hpp"

#include <cmath>
#include <set>

/*
 * This class creates an ImGui debug panel to check all logs during the execution.
 */
class IGDebug : private IGMenu {
  private:
    LogManager *log_manager = LogManager::instance();

    // Current log to filter
    std::set<LogType> active_log_filter;

    // Keep the focus on the last log
    bool track_logs = true;
    // Flag that tells if the logs has a defined size or not. (If disabled could bring to lack of performance)
    bool m_active_autoclear = log_manager->getAutoclear();

    static void HelpMarker(const char *desc) {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip()) {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

  public:
    IGDebug() {
        // default log filter
        this->active_log_filter.insert(logs::ERROR);
        this->active_log_filter.insert(logs::GENERAL_EVENT);
        this->active_log_filter.insert(logs::STATE);
    }

    inline virtual void render() override {
        ImGui::Begin("Debug Logger");
        ImGui::SeparatorText("Log Type Filter");
        ImGui::BeginGroup();

        // Prints all the filter available
        for (auto t : logs::log_types) {
            ImGui::PushID(&t);
            // If the filter is already activated, change the color of the button
            if (this->active_log_filter.find(t) != this->active_log_filter.end()) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0, 0, 1));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
            }
            // When button is clicked remove or add the filter to the Set
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
        ImGui::SeparatorText("Logs");
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1),
                                            ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * 10));
        if (ImGui::BeginChild("Log Messages", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                              ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {
            ImGui::PopStyleColor();

            // Start printing all logs every frame
            for (auto elem : log_manager->getLogs()) {
                if (this->active_log_filter.find(elem.getLogTypeObject()) != this->active_log_filter.end()) {
                    ImGui::PushID(&elem);
                    // Every Log has his color.
                    ImGui::TextColored(elem.getLogTypeColor(), "(%s) [%s]", elem.getLogTime(), elem.getLogType());
                    ImGui::SameLine();
                    ImGui::Text("=> %s", elem.getLogText());
                    ImGui::PopID();
                }
                // If track is enabled scroll to the last log in the set
                if (this->track_logs && log_manager->getTrack() == log_manager->getLogs().size() - 1) {
                    ImGui::SetScrollHereY();
                }
            }
        }
        ImGui::EndChild();
        // Enable/Disable auto scroll to last log
        ImGui::Checkbox("Track", &this->track_logs);
        if (ImGui::Button("Clear##1")) {
            log_manager->clear();
            log_manager->resetTrack();
        }
        ImGui::SameLine();
        // Enable/Disable autoclear feature
        if (ImGui::Checkbox("AutoClear", &this->m_active_autoclear)) {
            log_manager->setAutoclear(this->m_active_autoclear);
        }
        ImGui::SameLine();
        // Info popup
        IGDebug::HelpMarker("If you are experiencing some lag, it might be here the problem.\n"
                            "This Logger is a bit expensive, every frame writes all the logs.\n"
                            "So I implemented in a way that after \"50\" logs, it deletes the first log sent.\n"
                            "To disable this feature check the checkbox near clear, called \'AutoClear\'.\n"
                            "If you improve performance click the \'Clear\' button.");

        ImGui::End();
    }
};

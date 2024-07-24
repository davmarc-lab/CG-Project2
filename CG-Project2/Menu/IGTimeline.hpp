#pragma once

#include "IGMenu.hpp"

#include "Breakpoint/Breakpoint.hpp"

#include <vector>

class IGTimeline : public IGMenu {
  private:
    std::vector<Breakpoint *> steps;

  public:
    IGTimeline() {}

    inline virtual void render() override {
        ImGui::Begin("TimeLine");

        ImGui::End();
    }

    ~IGTimeline() = default;
};

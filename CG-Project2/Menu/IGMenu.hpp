#pragma once

#include "../LibCustomGui.hpp"

class IGMenu {
  private:
    /*
     * This interface provides method to render an ImGui window.
     */
  public:
    IGMenu() {}

    virtual void render() = 0;

    ~IGMenu() = default;
};

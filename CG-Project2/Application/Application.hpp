#pragma once

#include "../Lib.hpp"
#include <string>

class Application {

private:
    const float width;
    const float height;

    const string name;

public:
    Application() : Application(WIDTH, HEIGHT, "No Name") {}

    Application(const string name) : Application(WIDTH, HEIGHT, name) {}

    Application(const float width, const float height) : Application(width, height, "No Name") {}

    Application(const float width, const float height, const string name) : width(width), height(height), name(name) {}

    void launch();

    ~Application() = default;

};

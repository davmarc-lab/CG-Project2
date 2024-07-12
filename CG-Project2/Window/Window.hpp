#pragma once

#include <string.h>
#include "../Lib.hpp"

class Window
{
private:
    GLFWwindow *window;
    int width;
    int height;
    const char *windowName;
    vec2 resolution;

public:
    Window(const char *windowName, int w, int h);

    int initializeWindow();

    void closeWindow();

    void terminateWindow();

    inline char *getWindowName() { return (char *)this->windowName; };

    inline GLFWwindow *getWindow() { return this->window; };

    inline vec2 getResolution() { return this->resolution; }

    inline void enableGL(GLenum var) { glEnable(var); }

    ~Window() = default;
};

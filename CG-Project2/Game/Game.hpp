#pragma once

#include "../Window/Window.hpp"

class GameState;

class GameEngine {

public:
    GameEngine(Window *window, unsigned int width, unsigned int height);

    void init();
    void clean();

    void changeState(GameState *state);
    void PushState(GameState *state);
    void popState();

    void processInput();
    void update();
    void render();

    inline bool isRunning() { return this->running; }
    inline void quit() { this->running = false; }

    inline void setDeltaTime(const float dt) { this->deltaTime = dt; }

    inline float getDeltaTime() { return this->deltaTime; }

    inline Window *getWindow() { return this->window; }

    ~GameEngine() = default;

private:
    vector<GameState *> states;
    bool running = false;

    float deltaTime;
    unsigned int width, height;
    Window *window = nullptr;
};

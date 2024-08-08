#pragma once

#include "../Window/Window.hpp"

#include "../Action/ActionManager.hpp"

class GameState;

/*
 * This class provides method for GameLoop management.
 * It uses GameStates to manage different moment in the application.
 */
class GameEngine {

  public:
    GameEngine(Window *window, unsigned int width, unsigned int height);

    // Init phase.
    void init();
    // Cleaning phase.
    void clean();

    // Change the current state.
    void changeState(GameState *state);
    // Push the new state to the queue.
    void PushState(GameState *state);
    // Remove the old state from the queue.
    void popState();

    // Process user input.
    void processInput();
    // Update game entities.
    void update();
    // Render game entities.
    void render();

    inline bool isRunning() { return this->running; }

    // Exit from the GameLoop.
    inline void quit() { this->running = false; }

    // Sets the delta time (difference between the previous frame and the current frame).
    inline void setDeltaTime(const float dt) { this->deltaTime = dt; }

    inline float getDeltaTime() { return this->deltaTime; }

    inline Window *getWindow() { return this->window; }

    ~GameEngine() = default;

  private:
    // States queue.
    vector<GameState *> states;
    bool running = false;

    float deltaTime;
    unsigned int width, height;
    Window *window = nullptr;
};

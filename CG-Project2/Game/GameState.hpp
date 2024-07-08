#pragma once

#include "Game.hpp"

class GameState {
public:

    virtual void init() = 0;
    virtual void clean() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handleEvent(GameEngine* engine) = 0;
    virtual void update(GameEngine* engine) = 0;
    virtual void draw(GameEngine* engine) = 0;

    inline void changeState(GameEngine* engine, GameState* state) {
        engine->changeState(state);
    }

protected:
    GameState() {}
};

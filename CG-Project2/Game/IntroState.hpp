#pragma once

#include "GameState.hpp"

class IntroState : public GameState {

public:
    void init() override;
    void clean() override;

    void pause() override;
    void resume() override;

    void handleEvent(GameEngine *engine) override;
    void update(GameEngine *engine) override;
    void draw(GameEngine *engine) override;

    inline static IntroState* instance() {
        return &introState;
    }

protected:
    IntroState() {}
private:
    static IntroState introState;
};

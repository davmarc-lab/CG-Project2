#pragma once

#include "GameState.hpp"

class PlayState : public GameState {

public:
    void init() override;
    void clean() override;

    void pause() override;
    void resume() override;

    void handleEvent(GameEngine *engine) override;
    void update(GameEngine *engine) override;
    void draw(GameEngine *engine) override;

    inline static PlayState* instance() {
        return &playState;
    }

protected:
    PlayState() {}
private:
    static PlayState playState;
};


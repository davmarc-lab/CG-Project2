#pragma once

#include "GameState.hpp"

class OptionState : public GameState {

public:
    void init() override;
    void clean() override;

    void pause() override;
    void resume() override;

    void handleEvent(GameEngine *engine) override;
    void update(GameEngine *engine) override;
    void draw(GameEngine *engine) override;

    inline static OptionState* instance() {
        return &optionState;
    }

protected:
    OptionState() {}
private:
    static OptionState optionState;
};

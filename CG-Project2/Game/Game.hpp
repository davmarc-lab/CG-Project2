#pragma once

#include "../Lib.hpp"

/*
 * Enum to manage game phases.
 */
enum GameState
{
    GAME_NONE,
    GAME_MENU,
    GAME_ACTIVE,
    GAME_PAUSE,
    GAME_END
};

class Game {
    private:
        GameState state = GameState::GAME_NONE;
        ivec2 resolution = ivec2(0);

    public:
        Game() {}

        Game(const int width, const int height);

        void init();

        void processInput(int key, int mode, float dt);

        ~Game() = default;
};

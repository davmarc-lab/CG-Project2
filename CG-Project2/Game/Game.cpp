#include "Game.hpp"
#include "GameState.hpp"

#include "../Window/Window.hpp"

#include "../Menu/Logger/LogManager.hpp"

Mouse input;

GameEngine::GameEngine(Window *window, unsigned int width, unsigned int height) {
    this->window = window;
    this->width = width;
    this->height = height;
}

void GameEngine::init() { this->running = true; }

void GameEngine::changeState(GameState *state) {
    // empty state buffer
    if (!this->states.empty()) {
        this->states.back()->clean();
    }
    this->states.push_back(state);
    LogManager::instance()->addLog(logs::STATE, "Changing State");
    this->states.back()->init();
}

void GameEngine::processInput() { this->states.back()->handleEvent(this); }

void GameEngine::update() { this->states.back()->update(this); }

void GameEngine::render() {
    // draw the game scene
    this->states.back()->draw(this);
}

void GameEngine::clean() {}

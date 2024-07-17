#pragma once

#include "Action.hpp"

#include <vector>

class ActionManager {
private:
    std::vector<Action> actions_queue;

    static ActionManager *pointer;

protected:
    ActionManager() {}

public:
    ActionManager(const ActionManager &obj) = delete;

    inline static ActionManager *instance() {
        if (pointer == nullptr) {
            pointer = new ActionManager();
        }

        return pointer;
    }

    inline void addAction(Action action) { this->actions_queue.push_back(action); }

    inline std::vector<Action> getActions() { return this->actions_queue; }

    inline bool isActionPresent() { return this->actions_queue.size() > 0; }

    inline void clear() { this->actions_queue.clear(); }
};

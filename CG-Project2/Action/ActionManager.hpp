#pragma once

#include "Action.hpp"

#include <vector>

/*
 * This class uses a Singleton pattern to create only one instance in all the project.
 * ActionManager is responsible to store all the Actions that will be processed in the GameLoop.
 */
class ActionManager {
  private:
    std::vector<Action> actions_queue;

    static ActionManager *pointer;

  protected:
    ActionManager() {}

  public:
    ActionManager(const ActionManager &obj) = delete;

    /*
     * This static method retrieves the unique instance of this class.
     */
    inline static ActionManager *instance() {
        if (pointer == nullptr) {
            pointer = new ActionManager();
        }

        return pointer;
    }

    /*
     * Add an Action to the queue.
     */
    inline void addAction(Action action) { this->actions_queue.push_back(action); }

    /*
     * Retrieves all the Actions.
     */
    inline std::vector<Action> getActions() { return this->actions_queue; }

    /*
     * Retrieves true if there are Actions in the queue.
     */
    inline bool isActionPresent() { return this->actions_queue.size() > 0; }

    /*
     * Clean the Action queue, this method should be invoked at the end of every GameLoop iteration.
     */
    inline void clear() { this->actions_queue.clear(); }
};

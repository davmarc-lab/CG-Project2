#pragma once

#include "../Entity.hpp"

class Collision {
private:

    Entity* first;
    Entity* second;

public:
    Collision(Entity* first, Entity* second) : first(first), second(second) {}

    inline pair<Entity*, Entity*> getEntities() { return std::pair<Entity*, Entity*>(first, second); }

    ~Collision() = default;
};

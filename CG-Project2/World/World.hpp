#pragma once

#include "../Lib.hpp"

#include "../Buffers/VAO.hpp"
#include "../Entity/Entity.hpp"

#include <unordered_map>

class World {
    private:
        VAO vao;
        unsigned int currentId = 0;
        std::unordered_map<unsigned int, Entity> entities;

    public:
        World() {}

        void addEntity() {
            this->entities.insert(std::make_pair(this->currentId, Entity(to_string(this->currentId))));
            this->currentId++;
        }

        void addEntity(Entity entity) {
            this->entities.insert(std::make_pair(this->currentId, entity));
            this->currentId++;
        }

        std::unordered_map<unsigned int, Entity> getEntities() { return this->entities; }

        ~World() = default;
};

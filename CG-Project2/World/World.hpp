#pragma once

#include "../Lib.hpp"

#include "../Buffers/VAO.hpp"
#include "../Entity/Entity.hpp"

#include <unordered_map>

class World {
    private:
        unsigned int currentId = 0;
        VAO vao;
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

        GLuint getId() { return this->vao.getId(); }

        std::unordered_map<unsigned int, Entity> getEntities() { return this->entities; }

        Entity getEntityById(const unsigned int id) { return this->entities.find(id)->second; }

        ~World() = default;
};

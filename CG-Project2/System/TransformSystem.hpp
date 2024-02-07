#pragma once

#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "ISystem.hpp"

class TransformSystem : public ISystem {
    private:
        vector<Entity*> entities;

    public:
        void addEntity(Entity* entity) {
            this->entities.push_back(entity);
        }

        virtual void update() override {
            for (Entity* entity : entities) {
                Transform* transformComponent = entity->getComponent<Transform>();
                if (transformComponent) {
                    // tells to execute action
                    transformComponent->applyTransform();
                }
            }
        }

};

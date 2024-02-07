#pragma once

#include "../Lib.hpp"
#include "../Component/IComponent.hpp"

#include <unordered_map>

class Entity {
    private:
        string tag;

        // components for each entity
        unordered_map<std::string, IComponent*> components;
    public:
        Entity() {}

        Entity(string tag) : tag(tag) {}

        template <typename T> T* getComponent() {
            auto it = components.find(typeid(T).name());
            if (it != components.end()) {
                return dynamic_cast<T*>(it->second);
            }
            return nullptr;
        }

        template <typename T, typename... Args> void addComponent(Args&&... args) {
            T* component = new T(forward<Args>(args)...);
            this->components[typeid(T).name()] = component;
        }

        string getTag() { return this->tag; }

        void setTag(string tag) { this->tag = tag; }

        ~Entity() = default;
};

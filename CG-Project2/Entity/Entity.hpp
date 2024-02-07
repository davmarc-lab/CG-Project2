#pragma once

#include "../Lib.hpp"
#include "../Component/IComponent.hpp"

#include <unordered_map>
#include <utility>

class Entity {
    private:
        unordered_map<std::string, IComponent*> components;
    public:
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
};

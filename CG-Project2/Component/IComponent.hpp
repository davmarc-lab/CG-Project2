#pragma once

#include <typeinfo>

class IComponent {
    public:
        virtual ~IComponent() = default;

        const char* getComponentClass() { return typeid(this).name(); }
};

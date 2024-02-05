#pragma once

#include <typeinfo>
class IComponent {
    public:
        const char* getComponentClass() { return typeid(this).name(); }
};

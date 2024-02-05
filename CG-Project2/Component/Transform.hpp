#include "IComponent.hpp"

#include "../Lib.hpp"

struct Position {
    float x;
    float y;
    float z;
};

struct Scale {
    float x;
    float y;
    float z;
};

struct Rotation {
    float x;
    float y;
    float z;
};

class Transform : public IComponent {
    private:
        Position position;
        Scale scale;
        Rotation rotation;

    public:
        Transform(const vec3 position, const vec3 scale, const vec3 rotation);

        Transform() {}

        ~Transform();
};

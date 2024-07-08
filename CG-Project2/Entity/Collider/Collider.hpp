#pragma once

#include "../Transform/Transform.hpp"

class Collider {
protected:
    Transform* trasform;

    bool staticColl = false;

public:

    virtual void createCollider(const vector<vec3> coords) = 0;
    
    virtual bool testCollision(Transform* transform,
        Collider* collider,
        Transform* colliderTransform) = 0;

    inline void setAirColl() { this->staticColl = false; }

    inline void setStaticColl() { this->staticColl = true; }

    inline bool isStaticColl() { return this->staticColl; }

};


#pragma once

#include "Collider.hpp"

class SphereCollider : public Collider {
private:
    Transform *transform;

public:
    SphereCollider(Transform* transform) : transform(transform) {}

    virtual void createCollider(const vector<vec3> coords) {
        warning("No need to create a collider for a Sphere, get sphere radius and multiply by scale vector");
    }
    
    virtual bool testCollision(Transform* transform,
        Collider* collider,
        Transform* colliderTransform) {

        // all spheres radius are 1
        auto sphere = transform->getScale() * vec3(1.0f);

        // get botleft and topright, compare it with the distance between the center of the sphere

        return false;
    }

    ~SphereCollider() = default;
};

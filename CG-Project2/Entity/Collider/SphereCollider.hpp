#pragma once

#include "Collider.hpp"

#include "../../Menu/Logger/LogManager.hpp"

class SphereCollider : public Collider {
  private:
    Transform *transform;

  public:
    SphereCollider(Transform *transform) : transform(transform) {}

    virtual void createCollider(const vector<vec3> coords) {
        LogManager::instance()->addLog(logs::MISSING_IMPLEMENTATION, "No Implemetation for Sphere Collider");
    }

    virtual bool testCollision(Transform *transform, Collider *collider, Transform *colliderTransform) {

        // all spheres radius are 1
        auto sphere = transform->getScale() * vec3(1.0f);

        // get botleft and topright, compare it with the distance between the center of the sphere

        return false;
    }

    ~SphereCollider() = default;
};

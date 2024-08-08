#pragma once

#include "Collider.hpp"

struct AABB {
    vec3 botLeft;
    vec3 topRight;
};

class CubeCollider : public Collider {
private:
    BoundingBox box;

    AABB coll;

    Transform *transform;

public:
    CubeCollider(Transform *transform) : transform(transform) {}

    inline BoundingBox getBoundingBox() { return this->box; }

    inline void setBoundingBox(const vec3 botLeft, const vec3 topRight) { this->box = {botLeft, topRight}; }

    inline void setBoundingBox(const BoundingBox box) { this->box = box; }

    virtual void createCollider(const vector<vec3> coords) override {
        // get min coord
        vec3 botLeft = vec3(2);
        for (const auto p : coords) {
            if (p.x < botLeft.x) {
                botLeft.x = p.x;
            }
            if (p.y < botLeft.y) {
                botLeft.y = p.y;
            }
            if (p.z < botLeft.z) {
                botLeft.z = p.z;
            }
        }
        this->coll.botLeft = botLeft;
        // get max coord
        vec3 topRight = vec3(-2);
        for (const auto p : coords) {
            if (p.x > topRight.x) {
                topRight.x = p.x;
            }
            if (p.y > topRight.y) {
                topRight.y = p.y;
            }
            if (p.z > topRight.z) {
                topRight.z = p.z;
            }
        }
        this->coll.topRight = topRight;

        this->box = {this->transform->getModelMatrix() * vec4(botLeft, 1), this->transform->getModelMatrix() * vec4(topRight, 1)};
    }

    virtual bool testCollision(Transform *transform, Collider *collider, Transform *colliderTransform) override {
        this->box = {this->transform->getModelMatrix() * vec4(this->coll.botLeft, 1), this->transform->getModelMatrix() * vec4(this->coll.topRight, 1)};

        vec3 topPoint = ((CubeCollider *)collider)->getBoundingBox().topRight;
        vec3 botPoint = ((CubeCollider *)collider)->getBoundingBox().botLeft;

        bool botCollisionX = this->box.botLeft.x <= topPoint.x && this->box.topRight.x >= botPoint.x;
        bool botCollisionY = this->box.botLeft.y <= topPoint.y && this->box.topRight.y >= botPoint.y;
        bool botCollisionZ = this->box.botLeft.z <= topPoint.z && this->box.topRight.z >= botPoint.z;

        bool topCollisionX = this->box.botLeft.x <= topPoint.x && this->box.topRight.x >= botPoint.x;
        bool topCollisionY = this->box.botLeft.y <= topPoint.y && this->box.topRight.y >= botPoint.y;
        bool topCollisionZ = this->box.botLeft.z <= topPoint.z && this->box.topRight.z >= botPoint.z;

        return (botCollisionX && botCollisionY && botCollisionZ) && (topCollisionX && topCollisionY && topCollisionZ);
    }

    ~CubeCollider() = default;
};

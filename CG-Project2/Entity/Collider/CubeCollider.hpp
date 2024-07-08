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

        bool botCollisionX = this->box.botLeft.x - MARGIN_CUBE <= topPoint.x + MARGIN_CUBE && this->box.topRight.x - MARGIN_CUBE >= botPoint.x + MARGIN_CUBE;
        bool botCollisionY = this->box.botLeft.y - MARGIN_CUBE <= topPoint.y + MARGIN_CUBE && this->box.topRight.y - MARGIN_CUBE >= botPoint.y + MARGIN_CUBE;
        bool botCollisionZ = this->box.botLeft.z - MARGIN_CUBE <= topPoint.z + MARGIN_CUBE && this->box.topRight.z - MARGIN_CUBE >= botPoint.z + MARGIN_CUBE;

        bool topCollisionX = this->box.botLeft.x - MARGIN_CUBE <= topPoint.x + MARGIN_CUBE && this->box.topRight.x - MARGIN_CUBE >= botPoint.x + MARGIN_CUBE;
        bool topCollisionY = this->box.botLeft.y - MARGIN_CUBE <= topPoint.y + MARGIN_CUBE && this->box.topRight.y - MARGIN_CUBE >= botPoint.y + MARGIN_CUBE;
        bool topCollisionZ = this->box.botLeft.z - MARGIN_CUBE <= topPoint.z + MARGIN_CUBE && this->box.topRight.z - MARGIN_CUBE >= botPoint.z + MARGIN_CUBE;

        return (botCollisionX && botCollisionY && botCollisionZ) && (topCollisionX && topCollisionY && topCollisionZ);
    }

    ~CubeCollider() = default;
};

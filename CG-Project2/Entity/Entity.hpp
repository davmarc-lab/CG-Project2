#pragma once

#include "../Lib.hpp"

#include "../Shader/Shader.hpp"
#include "Textures/Texture.hpp"
#include "Transform/Transform.hpp"
#include "Collider/Collider.hpp"

class Entity {
public:

    Buffers buffers;

    Coords coords;

    Transform transform;

    Texture texture;

    Physic physics;
    bool m_static = false;

    Collider* collider;

    int nvertex;

    Entity() {}

    inline vec3 getMinVertex() {
        vec3 point = vec3(2);
        for (const auto p : this->coords.vertex) {
            if (p.x < point.x) {
                point.x = p.x;
            }
            if (p.y < point.y) {
                point.y = p.y;
            }
            if (p.z < point.z) {
                point.z = p.z;
            }
        }
        return this->getModelMatrix() * vec4(point, 1);
    }

    inline vec3 getMaxVertex() {
        vec3 point = vec3(-2);
        for (const auto p : this->coords.vertex) {
            if (p.x > point.x) {
                point.x = p.x;
            }
            if (p.y > point.y) {
                point.y = p.y;
            }
            if (p.z > point.z) {
                point.z = p.z;
            }
        }
        return this->getModelMatrix() * vec4(point, 1);
    }

    inline vec3 getPosition() { return this->transform.getPosition(); }

    inline void setPosition(const vec3 position) { this->transform.setPosition(position); }

    inline vec3 getScale() { return this->transform.getScale(); }

    inline void setScale(const vec3 scale) { this->transform.setScale(scale); }

    inline vec3 getRotation() { return this->transform.getRotation(); }

    inline void setRotation(const vec3 rotation) { this->transform.setRotation(rotation); }

    inline mat4 getModelMatrix() { return this->transform.getModelMatrix(); }

    inline mat4 refreshModelMatrix() { return this->transform.refreshModelMatrix(); }

    inline void setModelMatrix(const mat4 transform) { this->transform.setModelMatrix(transform); }

    virtual void applyTransformation(const vec3 translate, const vec3 scale, const vec3 rotAxis, float rotValue) = 0;

    inline Transform* getTransform() { return &this->transform; }

    inline float getMass() { return this->physics.mass; }

    inline void setMass(const float mass) { this->physics.mass = mass; }

    inline vec3 getForce() { return this->physics.force; }

    inline void setForce(const vec3 force) { this->physics.force = force; }

    inline void addForce(const vec3 force) { this->setForce(this->getForce() + force); }

    inline vec3 getAcceleration() { return this->physics.acceleration; }

    inline void setAcceleration(const vec3 acceleration) { this->physics.acceleration = acceleration; }

    inline void addAcceleration(const vec3 acceleration) { this->physics.acceleration += acceleration; }

    inline vec3 getVelocity() { return this->physics.velocity; }

    inline void setVelocity(const vec3 velocity) { this->physics.velocity = velocity; }

    inline void addVelocity(const vec3 velocity) { this->setVelocity(this->getVelocity() + velocity); }

    inline float getMotionTime() { return this->physics.motionTime; }

    inline void setMotionTime(const float time) { this->physics.motionTime = time; }

    inline void addMotionTime(const float time) { this->physics.motionTime += time; }

    inline Collider* getCollider() { return this->collider; }

    inline void setCollider(Collider* collider) {
        this->collider = collider;
        this->collider->createCollider(this->coords.vertex);
    }

    inline bool isCollStatic() { return this->collider->isStaticColl(); }

    inline void setCollStatic() { this->collider->setStaticColl(); }

    inline bool isStatic() { return this->m_static; }

    inline void setStatic() { this->m_static = true; }

    inline void setDynamic() { this->m_static = false; }

    inline Texture getTexture() { return this->texture; }

    inline void changeTexture(const string path) {
        this->texture = Texture(path.c_str());
        this->texture.createTexture();
        this->attachTexture(this->getTexture());
    }

    inline void attachTexture(Texture texture) {
        this->texture = texture;

        glBindVertexArray(this->buffers.vao);

        glGenBuffers(1, &this->buffers.vbo_t);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_t);
        glBufferData(GL_ARRAY_BUFFER, this->coords.texCoords.size() * sizeof(vec2),
                     this->coords.texCoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    virtual void createVertexArray() = 0;

    virtual void draw(Shader shader) = 0;

    // virtual void clear() = 0;

    ~Entity() = default;
};

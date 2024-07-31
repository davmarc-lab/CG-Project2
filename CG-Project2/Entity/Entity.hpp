#pragma once

#include "../Lib.hpp"

#include "../Shader/Shader.hpp"
#include "Collider/Collider.hpp"
#include "Material/Material.hpp"
#include "Textures/Texture.hpp"
#include "Transform/Transform.hpp"

class Entity {
<<<<<<< Updated upstream
public:

    // Stores all buffers to be used in the pipeline.
=======
  public:
>>>>>>> Stashed changes
    Buffers buffers;

    // Stores all vertices data.
    Coords coords;

    // Stores all transformation data (translation, scale, rotation).
    Transform transform;

    // Store Entity's Texture.
    Texture texture;

    // Store Entity's Material.
    Material material = material::NONE;

    // Define the Light complexity for each Entity.
    int light_comp = LightComp::PHONG;

<<<<<<< Updated upstream
    // Store Entity's Collider shape.
    Collider* collider;
=======
    Physic physics;
    bool m_static = false;

    Collider *collider;
>>>>>>> Stashed changes

    int nvertex;
    // Flag used to check if the Entity vao is already created.
    bool m_isInstaced = false;

    Entity() {}

    // Retrieves the minimum vertex for AABB collisions.
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

    // Retrieves the maximum vertex for AABB collisions.
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

    // Retrieves from the Transform object the updated model matrix using position, scale and rotation data.
    inline mat4 getModelMatrix() { return this->transform.getModelMatrix(); }

    inline void setModelMatrix(const mat4 transform) { this->transform.setModelMatrix(transform); }

    inline Transform *getTransform() { return &this->transform; }

<<<<<<< Updated upstream
    inline Collider* getCollider() { return this->collider; }
=======
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

    inline Collider *getCollider() { return this->collider; }
>>>>>>> Stashed changes

    inline void setCollider(Collider *collider) {
        this->collider = collider;
        this->collider->createCollider(this->coords.vertex);
    }

    inline bool isCollStatic() { return this->collider->isStaticColl(); }

    inline void setCollStatic() { this->collider->setStaticColl(); }

    // Retrieves the Entity texture.
    inline Texture getTexture() { return this->texture; }
    
    // Changes the current Texture.
    inline void changeTexture(const string path) {
        this->texture = Texture(path.c_str());
        this->texture.createTexture();
        this->attachTexture(this->getTexture());
    }

    // Attaches a Texture object to an Entity.
    inline void attachTexture(Texture texture) {
        this->texture = texture;

        glBindVertexArray(this->buffers.vao);

        glGenBuffers(1, &this->buffers.vbo_t);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_t);
        glBufferData(GL_ARRAY_BUFFER, this->coords.texCoords.size() * sizeof(vec2), this->coords.texCoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    inline Material getMaterial() { return this->material; }

    inline void setMaterial(Material material) { this->material = material; }
    
    // Retrieves true if the Entity should be affected by the lights in the Scene.
    inline bool isAffectedByLight() { return this->light_comp != LightComp::NONE; }

    inline void setLightComputation(int val) { this->light_comp = val; }

    inline int getLightComputation() { return this->light_comp; }

    inline bool isInstanced() { return this->m_isInstaced; }

    // Instances all the buffer in the gpu pipeline.
    virtual void createVertexArray() = 0;

    /*
     * Draws the Entity in the window, but before doing that it sends to the shader
     * all the information about texture and material for lighting.
     */
    virtual void draw(Shader shader) = 0;

    // virtual void clear() = 0;

    ~Entity() = default;
};

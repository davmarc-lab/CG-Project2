#pragma once

#include "../Lib.hpp"

#include "../Shader/Shader.hpp"
#include "Collider/Collider.hpp"
#include "Material/Material.hpp"
#include "Textures/Texture.hpp"
#include "Transform/Transform.hpp"

class Entity {
  public:

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
    int light_comp = LightComp::DEFAULT;

    // Store Entity's Collider shape.
    Collider* collider;

    int nvertex;
    // Flag used to check if the Entity vao is already created.
    bool m_isInstaced = false;

    bool show_wireframe = false;

    Physic m_physic;

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

    inline Collider* getCollider() { return this->collider; }

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
    inline bool isAffectedByLight() { return this->light_comp != LightComp::DEFAULT; }

    inline void setLightComputation(int val) { this->light_comp = val; }

    inline int getLightComputation() { return this->light_comp; }

    inline bool isInstanced() { return this->m_isInstaced; }

    inline bool isShowingWireframe() { return this->show_wireframe; }

    inline void showWireframe(bool val) { this->show_wireframe = val; }

    inline void addVelocity(float vel) { this->m_physic.velocity += vel; }

    inline void setvelocity(float vel) { this->m_physic.velocity = vel; }

    inline float getVelocity() { return this->m_physic.velocity; }

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

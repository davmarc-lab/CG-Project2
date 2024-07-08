#pragma once

#include "Entity.hpp"
#include "../Color/Color.hpp"

class Sphere : public Entity {
private:
    const int slices;
    const int stacks;

    vector<GLuint> index;
    Color color = color::BLACK;

    void buildSphere();

public:

    Sphere(const int slices = 30, const int stacks = 30) : slices(slices), stacks(stacks) {
        this->buildSphere();
    }

    inline vec3 getRadius() { return vec3(1.0f); }
    
    inline virtual void applyTransformation(const vec3 translate, const vec3 scale, const vec3 rotAxis, float rotValue) override {
        this->setPosition(this->getPosition() + translate);
        this->setScale(scale);
        this->setRotation(this->getRotation() + rotAxis * rotValue);
        this->transform.applyTransofrmation(translate, scale, rotAxis, rotValue);
    }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~Sphere() = default;

};

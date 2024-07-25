#pragma once

#include "../Color/Color.hpp"
#include "Entity.hpp"

class Sphere : public Entity {
  private:
    const int slices;
    const int stacks;

    vector<GLuint> index;
    Color color = color::BLACK;

    void buildSphere();

  public:
    Sphere(const int slices = 30, const int stacks = 30) : slices(slices), stacks(stacks) { this->buildSphere(); }

    inline vec3 getRadius() { return vec3(1.0f); }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    ~Sphere() = default;
};

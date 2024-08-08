#pragma once

#include "../Entity/Entity.hpp"
#include "../Shader/Shader.hpp"

#include "../Light/Light.hpp"

#include <cassert>
#include <vector>

/*
 * This class create a Scene containing each entity, shader and light in the viewport.
 */
class Scene {
  private:
    // Stores all the entity with the corrisponding shader to be used
    vector<std::pair<Entity *, Shader *>> elements;

    // Vector used to draw lights caster
    vector<Light *> lights;

  public:
    Scene() {}

    // Add an entity in the scene.
    void addElement(Entity *e, Shader *shader);

    // Remove an entity or a lights from the scene.
    // Retrieves true if the element was erased.
    bool removeElement(Entity *e, Shader *s, bool is_light_selected, Light *light_selected);

    inline vector<std::pair<Entity *, Shader *>> getElements() { return this->elements; }

    void addLight(Light *l);

    inline vector<Light *> getLights() { return this->lights; }

    // Draw all the elements in the scene.
    void draw();

    inline void clear() {
        for (auto elem : this->elements) {
            // elem.first->clear();
            elem.second->clear();
        }

        this->elements.clear();
        this->lights.clear();
    }

    ~Scene() = default;
};

#pragma once

#include "../Entity/Entity.hpp"
#include "../Entity/Object.hpp"
#include "../Shader/Shader.hpp"

#include "../Light/Light.hpp"

#include <cassert>
#include <vector>

class Scene {
  private:
    vector<std::pair<Entity *, Shader *>> elements;
    vector<std::pair<Object *, Shader *>> custom_obj;

    vector<Light *> lights;

  public:
    Scene() {}

    void addElement(Entity *e, Shader *shader);

    void addCustomObj(Object *m, Shader *shader);

    bool removeElement(Entity *e, Shader *s, bool is_light_selected, Light *light_selected);

    inline vector<std::pair<Entity *, Shader *>> getElements() { return this->elements; }

    void addLight(Light *l);

    inline vector<Light *> getLights() { return this->lights; }

    void draw();

    inline void clear() {
        for (auto elem : this->elements) {
            // elem.first->clear();
            elem.second->clear();
        }

        this->elements.clear();
        this->custom_obj.clear();
        this->lights.clear();
    }

    ~Scene() = default;
};

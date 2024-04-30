#include "Scene.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

Scene::Scene(mat4 projection)
{
    this->projection = mat4(projection);
}

void Scene::drawScene()
{
    for (int i = 0; i < this->getSceneElements().size(); i++)
    {
        auto shape = this->getSceneElements()[i].shape;
        auto shader = this->getSceneElements()[i].shader;

        shader.use();

        shader.setMat4("projection", this->projection);
        shape->draw(shader);
    }
}

void Scene::clear()
{
    for (auto elem: this->getSceneElements())
    {
        elem.shape->clearShape();
        elem.shader.clearShader();
    }
}

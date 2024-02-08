#include "Game.hpp"

#include "../World/World.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Render.hpp"
#include "../Shader/Shader.hpp"
#include "../System/RenderSystem.hpp"

World world = World();

RenderSystem rs;
Shader shader = Shader("resources/defaultVertexShader.glsl", "resources/defaultFragmentShader.glsl");

void Game::init() {
    Entity first = Entity("first");

    Transform trf = Transform(vec3(0), vec3(100), vec3(0));
    trf.applyTransform();
    rs = RenderSystem(shader, trf.getModelMatrix());
    first.addComponent<Transform>(trf);

    vector<vec3> vert;
    vert.push_back(vec3(-1, -1, 0));
    vert.push_back(vec3(1, -1, 0));
    vert.push_back(vec3(0.5, 1, 0));

    vector<vec4> colo;
    colo.push_back(vec4(1, 0, 0, 1));
    colo.push_back(vec4(1, 0, 0, 1));
    colo.push_back(vec4(1, 0, 0, 1));

    first.addComponent<Render>(vert, colo, 1);

    world.addEntity(first);

    /* for (std::pair<unsigned int, Entity> elem: tmp) { */
    /*     cout << "Key: " << elem.first << ", Value: " << elem.second.getTag() << endl; */
    /* } */

}

void Game::processInput(int key, int action, float dt) {

}


void Game::update(float dt) {
    mat4 projection = glm::ortho(0.f, (float)this->resolution.x, 0.f, (float)this->resolution.y, 0.1f, 100.f);
    shader.use();

    shader.setMat4("proj", projection);

    auto ent = world.getEntityById(0);
    auto comp = ent.getComponent<Transform>();

    rs.exec(world);

}

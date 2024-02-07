#include "Game.hpp"

#include "../World/World.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Render.hpp"

void Game::init() {
    World world = World();
    Entity first = Entity("first");

    world.addEntity(first);
    first.addComponent<Transform>();

    vector<vec3> vert;
    vert.push_back(vec3(-1, -1, 0));
    vert.push_back(vec3(1, -1, 0));
    vert.push_back(vec3(0.5, 1, 0));

    vector<vec4> colo;
    colo.push_back(vec4(1, 0, 0, 1));
    colo.push_back(vec4(1, 0, 0, 1));
    colo.push_back(vec4(1, 0, 0, 1));

    first.addComponent<Render>(vert, colo, 1);

    auto tmp = world.getEntities();

    /* for (std::pair<unsigned int, Entity> elem: tmp) { */
    /*     cout << "Key: " << elem.first << ", Value: " << elem.second.getTag() << endl; */
    /* } */

}

void Game::processInput(int key, int action, float dt) {

}

void Game::update(float dt) {

}

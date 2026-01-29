#include "../../Opengl-Core/include/Core.hpp"

#include <fstream>
#include <json/value.h>
#include <memory>
#include <vector>

#include "../include/Factory.hpp"
#include "../include/MeshLoader.hpp"

#include "../include/ECS/EntityManager.hpp"

#include "../include/Utils.hpp"

const auto em = EntityManager::instance();
const auto rd = ogl::Renderer::instance();

std::vector<unsigned int> MeshLoader::loadMeshes(const std::string &path) {
    return {};
}

bool MeshLoader::saveMeshes(std::vector<unsigned int> &entities) {
    return false;
}

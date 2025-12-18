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

template <typename T>
Shared<T> safeAddComponent(unsigned int &id, Json::Value &val, const std::string &name) {
	if (val[name] != Json::nullValue) {
		auto c = CreateShared<T>();
		c->deserialize(val[name]);
		em->addComponent(id, c);
		return c;
	}
	return nullptr;
}

std::vector<unsigned int> MeshLoader::loadMeshes(const std::string &path) {
	// with dynamic pointer cast throw error at runtime
	auto meshes = JsonSerializer::instance()->deserializeFromFile(path);
	std::vector<unsigned int> newEntities{};

	for (auto e : meshes) {
		if (e == Json::nullValue)
			continue;

		// create each entity
		auto id = em->createEntity();
		newEntities.push_back(id);
		safeAddComponent<Transform>(id, e, "transform");
		safeAddComponent<MultiMesh>(id, e, "multimesh");
		auto v = safeAddComponent<VertexComponent>(id, e, "vertex");
		safeAddComponent<TextureComponent>(id, e, "texture");
		safeAddComponent<ParentComponent>(id, e, "parent");
		safeAddComponent<ShaderComponent>(id, e, "shader");
		auto r = safeAddComponent<RenderComponent>(id, e, "render");
		safeAddComponent<MaterialComponent>(id, e, "material");
		safeAddComponent<ColliderComponent>(id, e, "collider");

		if (v != nullptr && r != nullptr) {
			// if it's renderable it can be loaded
			em->addComponent<LoaderComponent>(id);
			// create a buffer component
			auto b = em->addComponent<BufferComponent>(id);
			factory::fillBufferData(id);
			auto vaoid = b->vao.getId();
			r->setRenderCall([v, vaoid]() {
				rd->drawElements(vaoid, GL_TRIANGLES, v->getIndexCoords().size(), GL_UNSIGNED_INT);
			});
		}
	}

	return newEntities;
}

bool MeshLoader::saveMeshes(std::vector<unsigned int> &entities) {
	if (entities.size() == 0)
		return false;

	Json::Value arr(Json::arrayValue);
	Json::Value elem;

	// with static pointer cast throw error at compile time
	// with dynamic pointer cast throw error at runtime
	for (auto ent : entities) {
		// throw error if the entity should not be loaded
		ASSERT(em->entityHasComponent<LoaderComponent>(ent));

		elem = Json::nullValue;
		for (auto c : em->getEntityComponents(ent)) {
			auto casted = std::dynamic_pointer_cast<JsonSerializable>(c);
			if (casted != nullptr)
				elem[casted->getName()] = casted->serialize();
		}
		arr.append(elem);
	}
	JsonSerializer::instance()->serializeToFile(arr, "./resources/mesh/scene.json");
	return arr.size() > 0;
}

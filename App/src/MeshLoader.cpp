#include "../../Opengl-Core/include/Core.hpp"

#include "../include/MeshLoader.hpp"

#include "../include/Utils.hpp"

#include <fstream>
#include <iostream>

#include <json/json.h>

std::vector<std::string> MeshLoader::removeSpaces(const std::string &content) {
	std::vector<std::string> res{};
	std::string copy{content};
	std::string buff{};
	size_t index{};
	while ((index = copy.find(" ")) != std::string::npos) {
		buff = copy.substr(0, index);
		copy = copy.substr(index + 1);
		res.push_back(buff);
	}
	if (!copy.empty())
		res.push_back(copy);
	return res;
}

InfoEntity MeshLoader::parseEntityInfo(const std::string &content) {
	const size_t posIndex = content.find(FILE_POS);
	const size_t sclIndex = content.find(FILE_SCL);
	const size_t rotIndex = content.find(FILE_ROT);

	auto posString = content.substr(posIndex + FILE_POS.length() + 1, sclIndex - (posIndex + FILE_POS.length() + 1));
	auto sclString = content.substr(sclIndex + FILE_SCL.length() + 1, rotIndex - (sclIndex + FILE_SCL.length() + 1));
	auto rotString = content.substr(rotIndex + FILE_ROT.length() + 1);

	InfoEntity ent{};
	{
		const auto vals = removeSpaces(posString);
		ASSERT(vals.size() == 3);
		ent.pos.x = std::stof(vals.at(0));
		ent.pos.y = std::stof(vals.at(1));
		ent.pos.z = std::stof(vals.at(2));
	}
	{
		const auto vals = removeSpaces(sclString);
		ASSERT(vals.size() == 3);
		ent.scale.x = std::stof(vals.at(0));
		ent.scale.y = std::stof(vals.at(1));
		ent.scale.z = std::stof(vals.at(2));
	}
	{
		const auto vals = removeSpaces(rotString);
		ASSERT(vals.size() == 3);
		ent.rot.x = std::stof(vals.at(0));
		ent.rot.y = std::stof(vals.at(1));
		ent.rot.z = std::stof(vals.at(2));
	}
	return ent;
}

InfoMesh MeshLoader::unpackBuffer(const std::vector<std::string> &buffer) {
	InfoMesh res{};

	auto vert = std::find(ALL(buffer), FILE_MESH_VERT);
	if (vert != buffer.end()) {
		for (int i = 1; i <= std::atoi((vert + 1)->data()); i++) {
			auto content = removeSpaces(*(vert + 1 + i));
			ASSERT(content.size() == 3);
			res.vertex.emplace_back(std::stof(content.at(0)), std::stof(content.at(1)), std::stof(content.at(2)));
		}
	}

	auto color = std::find(ALL(buffer), FILE_MESH_COLOR);
	if (color != buffer.end()) {
		for (int i = 1; i <= std::atoi((color + 1)->data()); i++) {
			auto content = removeSpaces(*(color + 1 + i));
			ASSERT(content.size() == 4);
			res.colors.emplace_back(std::stof(content.at(0)), std::stof(content.at(1)), std::stof(content.at(2)), std::stof(content.at(3)));
		}
	}

	auto index = std::find(ALL(buffer), FILE_MESH_IDX);
	if (index != buffer.end()) {
		for (int i = 1; i <= std::atoi((index + 1)->data()); i++) {
			auto content = removeSpaces(*(index + 1 + i));
			ASSERT(content.size() == 1);
			res.index.emplace_back(std::atoi(content.at(0).c_str()));
		}
	}

	auto normal = std::find(ALL(buffer), FILE_MESH_NORM);
	if (normal != buffer.end()) {
		for (int i = 1; i <= std::atoi((normal + 1)->data()); i++) {
			auto content = removeSpaces(*(normal + 1 + i));
			ASSERT(content.size() == 3);
			res.normals.emplace_back(std::stof(content.at(0)), std::stof(content.at(1)), std::stof(content.at(2)));
		}
	}

	auto render = std::find(ALL(buffer), FILE_RENDER);
	if (render != buffer.end()) {
		auto content = removeSpaces(*(render + 1));
		ASSERT(content.size() == 3);
		// res.render = bolt::RenderHelper{(bolt::RenderType)std::atoi(content.at(0).c_str()), (u32)std::atoi(content.at(1).c_str()), (u32)std::atoi(content.at(2).c_str())};
	}

	return res;
}

enum State {
	READ_ENTITY,
	READ_INFO,
    READ_COMPONENTS,
	READ_MESH,
};

struct MeshHelper {
	/// mesh position
	glm::vec3 position{};
	/// mesh scale
	glm::vec3 scale{1, 1, 1};
	/// mesh rotation
	glm::vec3 rotation{};
	std::vector<glm::vec3> vertex = {};
	std::vector<glm::vec4> colors = {};
	std::vector<unsigned int> index = {};
	std::vector<glm::vec3> normals = {};
	std::vector<glm::vec2> texCoords = {};
};

enum RenderType {
	render_arrays,
	render_elements,
	render_instance // TODO
};

struct RenderHelper {
	RenderType type = render_arrays;
	unsigned int mode = 0;
	unsigned int first = 0;
};

void MeshLoader::loadMeshFromFile(const std::string &path) {
	// clear the current scene
	// CLEAR THE ECS TOO
	// bolt::Scene::instance()->clear();
	{
		std::ifstream file(path);
		std::string content{};
		auto state = READ_ENTITY;

		std::vector<std::string> mesh{};
		MeshHelper helper{};
		while (std::getline(file, content)) {
			switch (state) {
				case READ_ENTITY: {
					std::cout << "New Entity found with ID => " << content << "\n";
					helper = MeshHelper{};
					state = READ_INFO;
					break;
				}
				case READ_INFO: {
					auto info = MeshLoader::parseEntityInfo(content);
					helper.position = std::move(info.pos);
					helper.scale = std::move(info.scale);
					helper.rotation = std::move(info.rot);

					state = READ_MESH;
					break;
				}
				case READ_MESH: {
					if (content == "\0") {
						// parse all the vertices
						auto buff = unpackBuffer(mesh);
						if (!buff.vertex.empty())
							helper.vertex = std::move(buff.vertex);
						if (!buff.colors.empty())
							helper.colors = std::move(buff.colors);
						if (!buff.index.empty())
							helper.index = std::move(buff.index);
						if (!buff.normals.empty())
							helper.normals = std::move(buff.normals);
						if (!buff.texCoords.empty())
							helper.texCoords = std::move(buff.texCoords);

						// helper.renderInfo = buff.render;

						mesh.clear();
						state = READ_ENTITY;
						break;
					}
					mesh.push_back(content);
					break;
				}
			}
		}
	}
}

void MeshLoader::saveMeshToFile(const std::string &path) {
	// create file
	{
		std::ofstream file(path);
		int index = 0;
		// for (auto id : bolt::EntityManager::instance()->getEntitiesFromComponent<bolt::Mesh>()) {
		// 	file << FILE_NEW_ENTITY_SEP << index << "\n";
		// 	auto t = bolt::EntityManager::instance()->getEntityComponent<bolt::Transform>(id);
		// 	auto m = bolt::EntityManager::instance()->getEntityComponent<bolt::Mesh>(id);
		// 	auto pos = t->getPosition();
		// 	auto scale = t->getScale();
		// 	auto rot = t->getRotation();
		//
		// 	// model info
		// 	file << FILE_POS << " " << pos.x << " " << pos.y << " " << pos.z << " ";
		// 	file << FILE_SCL << " " << scale.x << " " << scale.y << " " << scale.z << " ";
		// 	file << FILE_ROT << " " << rot.x << " " << rot.y << " " << rot.z << "\n";
		//
		// 	file << FILE_RENDER << "\n";
		// 	file << "\t" << m->render.info.type << " " << m->render.info.mode << " " << m->render.info.first << "\n";
		//
		// 	// vertex info
		// 	if (!m->vertices.empty()) {
		// 		file << FILE_MESH_VERT << "\n";
		// 		file << m->vertices.size() << "\n";
		// 		for (auto v : m->vertices) {
		// 			file << "\t" << v.x << " " << v.y << " " << v.z << "\n";
		// 		}
		// 	}
		//
		// 	if (!m->colorComponent.colors.empty()) {
		// 		file << FILE_MESH_COLOR << "\n";
		// 		file << m->colorComponent.colors.size() << "\n";
		// 		for (auto v : m->colorComponent.colors) {
		// 			file << "\t" << v.x << " " << v.y << " " << v.z << " " << v.a << "\n";
		// 		}
		// 	}
		//
		// 	if (!m->indices.empty()) {
		// 		file << FILE_MESH_IDX << "\n";
		// 		file << m->indices.size() << "\n";
		// 		for (auto v : m->indices) {
		// 			file << "\t" << v << "\n";
		// 		}
		// 	}
		//
		// 	file << "\n";
		// 	index++;
		// }
		file.close();
	}
}

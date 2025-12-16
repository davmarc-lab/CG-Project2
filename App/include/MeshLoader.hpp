#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <string>

struct InfoEntity {
	glm::vec3 pos{};
	glm::vec3 scale{};
	glm::vec3 rot{};
};

struct InfoMesh {
	std::vector<glm::vec3> vertex{};
	std::vector<unsigned int> index{};
	std::vector<glm::vec4> colors{};
	std::vector<glm::vec3> normals{};
	std::vector<glm::vec2> texCoords{};
};

class MeshLoader {
public:
	static constexpr char FILE_NEW_ENTITY_SEP = '$';
	inline static const std::string FILE_POS = "pos";
	inline static const std::string FILE_SCL = "scale";
	inline static const std::string FILE_ROT = "rot";
	inline static const std::string FILE_MESH_VERT = "vertex";
	inline static const std::string FILE_MESH_IDX = "indices";
	inline static const std::string FILE_MESH_COLOR = "colors";
	inline static const std::string FILE_MESH_NORM = "normals";	 // TODO
	inline static const std::string FILE_MESH_TEX = "texCoords"; // TODO
	inline static const std::string FILE_RENDER = "render";

	static void loadMeshFromFile(const std::string &path);

	static void saveMeshToFile(const std::string &path = "config.txt");

private:
	static std::vector<std::string> removeSpaces(const std::string &content);

	static InfoEntity parseEntityInfo(const std::string &content);

	static InfoMesh unpackBuffer(const std::vector<std::string> &buffer);
};

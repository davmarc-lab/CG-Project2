#include "../include/Factory.hpp"

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../../Opengl-Core/include/Core.hpp"

#include "../../Opengl-Core/vendor/include/assimp/Importer.hpp"
#include "../../Opengl-Core/vendor/include/assimp/postprocess.h"
#include "../../Opengl-Core/vendor/include/assimp/scene.h"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <vector>

#include "../include/Utils.hpp"

const auto em = EntityManager::instance();
const auto rd = ogl::Renderer::instance();

namespace factory {
	unsigned int factoryCube(const BasicInfo &info, const glm::vec4 &color) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		auto vc = em->addComponent<VertexComponent>(id, cubeGeometry, getColorVector(color, cubeGeometry.size()), cubeIndices);
		vc->setNormalsCoords(cubeNormals);
		vc->setTexCoords(cubeTexCoord);
		auto bc = em->addComponent<BufferComponent>(id);
		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_c.onAttach();
		bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});

		return id;
	}

	unsigned int factorySphere(const BasicInfo &info, const glm::vec4 &color) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		auto coords = getSphereVertices();
		auto vc = em->addComponent<VertexComponent>(id, coords.vertex, getColorVector(color, coords.vertex.size()), coords.indices);
		vc->setNormalsCoords(coords.normals);
		vc->setTexCoords(coords.texCoords);
		auto bc = em->addComponent<BufferComponent>(id);
		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_c.onAttach();
		bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
		return id;
	}

	unsigned int factoryPyramid(const BasicInfo &info, const glm::vec4 &color) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		auto vc = em->addComponent<VertexComponent>(id, pyramidGeometry, getColorVector(color, pyramidGeometry.size()), pyramidIndices);
		vc->setNormalsCoords(pyramidNormals);
		vc->setTexCoords(pyramidTexCoords);
		auto bc = em->addComponent<BufferComponent>(id);
		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_c.onAttach();
		bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
		return id;
	}

	unsigned int factoryThorus(const BasicInfo &info, const glm::vec4 &color) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		auto coords = getThorusVertices();
		auto vc = em->addComponent<VertexComponent>(id, coords.vertex, getColorVector(color, coords.vertex.size()), coords.indices);
		vc->setNormalsCoords(coords.normals);
		vc->setTexCoords(coords.texCoords);
		auto bc = em->addComponent<BufferComponent>(id);
		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_c.onAttach();
		bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
		return id;
	}

	unsigned int factoryPlane(const glm::vec4 &color) {
		auto id = factoryCube(BasicInfo{{0, -2, 0}, {100, 0, 100}}, color);
		em->removeComponent<MaterialComponent>(id);
		return id;
	}

	std::vector<std::string> faces{"right", "left", "top", "bottom", "front", "back"};

	unsigned int factorySkyBox(const std::string &path, const std::string &format) {
		auto id = em->createEntity();
		::systems::ecs::updateEntityName(id, "Skybox");
		auto vc = em->addComponent<VertexComponent>(id, skyboxGeometry, std::vector<glm::vec4>{}, std::vector<unsigned int>{});
		auto bc = em->addComponent<BufferComponent>(id);

		// Buffers
		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		// Texture
		auto tc = em->addComponent<TextureComponent>(id);
		TextureParams params{};
		params.target = GL_TEXTURE_CUBE_MAP;
		ogl::Texture t{params, 0, 0};
		t.onAttach();
		t.bind();
		int width, height, nrChannels;
		for (size_t i = 0; i < faces.size(); i++) {
			auto dataRead = readImageData(path + faces[i] + "." + format, width, height, nrChannels, 0);
			if (dataRead)
				t.fastCreateCustomTexture2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataRead);
			else
				std::cerr << "Failed to read data from the file: " << path + faces[i] + "." + format << "\n";
			freeImageData(dataRead);
		}
		t.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		t.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		t.setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		t.setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		t.setTexParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		systems::texture::setTexture(id, t);

		em->addComponent<SkyboxComponent>(id, t.getId());

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		auto tid = t.getId();
		rc->setRenderCall([vc, vaoid, tid]() {
			int pCull;
			glGetIntegerv(GL_CULL_FACE, &pCull);
			glDisable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);
			glBindVertexArray(vaoid);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
			rd->drawArrays(vaoid, GL_TRIANGLES, 0, vc->getVertexCoords().size());
			glDepthFunc(GL_LESS);
			if (pCull)
				glEnable(GL_CULL_FACE);
		});
		return id;
	}

	unsigned int mainMesh{};

	unsigned int textureFromFile(const char *path, const std::string &dir) {
		std::string filename = std::string(path);
		filename = dir + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		auto data = readImageData(filename, width, height, nrComponents, 0);
		if (data) {
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		} else {
			std::cerr << "Texture failed to load at path: " << path << "\n";
		}
		freeImageData(data);

		return textureID;
	}

	std::vector<ImportedTexture> loadMaterialTextures(const aiMaterial *mat, const aiTextureType &type, const std::string &typeName, const std::string &dir) {
		std::vector<ImportedTexture> textures{};
		auto tt = em->getComponentFromId<ImportedMeshTextures>(mainMesh);
		ASSERT(tt != nullptr);

		for (auto i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (int j = 0; j < tt->textures.size(); j++) {
				if (std::strcmp(tt->textures[j].path.data(), str.C_Str()) == 0) {
					textures.push_back(tt->textures[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				ImportedTexture texture{};
				texture.id = textureFromFile(str.C_Str(), dir);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				tt->textures.push_back(texture);
			}
		}
		return textures;
	}

	unsigned int instanceMesh(const aiMesh *mesh, const aiScene *scene, const std::string &dir) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		em->addComponent<ColliderComponent>(id);
		em->addComponent<HideTreeComponent>(id);
		em->addComponent<ImportedMeshTextures>(id);
		MeshInfo info{};
		for (auto i = 0; i < mesh->mNumVertices; i++) {
			info.vertex.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if (mesh->HasNormals())
				info.normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0]) {
				info.texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
		}

		for (auto i = 0; i < mesh->mNumFaces; i++) {
			auto face = mesh->mFaces[i];
			for (auto j = 0; j < face.mNumIndices; j++) {
				info.indices.push_back(face.mIndices[j]);
			}
		}

		auto vc = em->addComponent<VertexComponent>(id, info.vertex, getColorVector({1, 0, 0, 1}, info.vertex.size()), info.indices);
		auto bc = em->addComponent<BufferComponent>(id);
		vc->setTexCoords(info.texCoords);

		if (mesh->mMaterialIndex >= 0) {
			auto material = scene->mMaterials[mesh->mMaterialIndex];
			auto tt = em->getComponentFromId<ImportedMeshTextures>(id);
			ASSERT(tt != nullptr);

			std::vector<ImportedTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", dir);
			tt->textures.insert(tt->textures.end(), ALL(diffuseMaps));

			std::vector<ImportedTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", dir);
			tt->textures.insert(tt->textures.end(), ALL(specularMaps));

			std::vector<ImportedTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", dir);
			tt->textures.insert(tt->textures.end(), ALL(specularMaps));

			std::vector<ImportedTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", dir);
			tt->textures.insert(tt->textures.end(), ALL(heightMaps));
		}

		bc->vao.onAttach();
		bc->vao.bind();

		bc->vbo_g.onAttach();
		bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_c.onAttach();
		bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_n.onAttach();
		bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->vbo_t.onAttach();
		bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
		bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vaoid, vc]() {
			// set texture units
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
		return id;
	}

	void processNode(unsigned int parent, const aiNode *node, const aiScene *scene, const std::string &dir) {
		ASSERT(node->mNumMeshes <= 1);
		unsigned int id;
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			id = instanceMesh(scene->mMeshes[node->mMeshes[i]], scene, dir);
			systems::parent::addChild(parent, id);
			parent = id;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(parent, node->mChildren[i], scene, dir);
		}
	}

	unsigned int factoryObjMesh(const BasicInfo &info, const std::string &pathToFile) {
		auto id = em->createEntity();
		mainMesh = id;
		em->addComponent<ParentComponent>(id);
		em->addComponent<Transform>(id);
		em->addComponent<ImportedMeshTextures>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		Assimp::Importer import{};
		const auto *scene = import.ReadFile(pathToFile, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
			return -1;
		}

		auto dir = pathToFile.substr(0, pathToFile.find_last_of('/'));
		processNode(id, scene->mRootNode, scene, dir);

		return id;
	}

	namespace light {
		unsigned int factoryDirectional(const glm::vec3 &direction) {
			auto id = em->createEntity();
			em->addComponent<LightComponent>(id, direction);
			return id;
		}

		unsigned int factoryPoint(const glm::vec3 &position, const LightConstraint &constraint) {
			auto id = em->createEntity();
			em->addComponent<LightComponent>(id, position, constraint);
			return id;
		}

		unsigned int factorySpot(const glm::vec3 &position, const glm::vec3 &direction, const LightConstraint &constraint, const float &cutOff, const float &outerCutOff) {
			auto id = em->createEntity();
			em->addComponent<LightComponent>(id, position, direction, constraint, cutOff, outerCutOff);
			return id;
		}
	} // namespace light

} // namespace factory

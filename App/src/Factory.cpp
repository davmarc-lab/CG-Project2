#include "../include/Factory.hpp"

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <vector>

#include "../include/Utils.hpp"

const auto em = EntityManager::instance();
const auto rd = ogl::Renderer::instance();

struct InstanceData {
	unsigned int nspheres = 0;
	std::vector<unsigned int> spheresIds{};
	std::vector<glm::vec4> spheresColor{};
	std::vector<glm::mat4> spheresModel{};
} data;

namespace factory {
	void fillBufferData(const unsigned int &id) {
		ASSERT(em->entityHasComponent<VertexComponent>(id));
		ASSERT(em->entityHasComponent<BufferComponent>(id));

		auto vc = em->getComponentFromId<VertexComponent>(id);
		auto bc = em->getComponentFromId<BufferComponent>(id);
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
	}

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

		fillBufferData(id);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		if (info.render) {
			rc->setRenderCall([vc, vaoid]() {
				rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
			});
		} else {
			rc->setRenderCall([]() {});
		}

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

		fillBufferData(id);

		// data.nspheres++;
		// data.spheresIds.push_back(id);
		// data.spheresColor.push_back(color);
		// data.spheresModel.push_back(systems::transform::getModelMatrix(id));
		// bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		//
		// bc->vbo_co.onAttach();
		// bc->vbo_co.setup(data.spheresColor.data(), data.spheresColor.size(), GL_STATIC_DRAW);
		// bc->vao.linkAttribFast(4, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		// glVertexAttribDivisor(4, 1);
		//
		// bc->vbo_mo.onAttach();
		// bc->vbo_mo.setup(data.spheresModel.data(), data.spheresModel.size(), GL_STATIC_DRAW);
		// bc->vao.linkAttribFast(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)0);
		// glVertexAttribDivisor(5, 1);
		// bc->vao.linkAttribFast(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(sizeof(glm::vec4)));
		// glVertexAttribDivisor(6, 1);
		// bc->vao.linkAttribFast(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(2 * sizeof(glm::vec4)));
		// glVertexAttribDivisor(7, 1);
		// bc->vao.linkAttribFast(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(3 * sizeof(glm::vec4)));
		// glVertexAttribDivisor(8, 1);

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

		fillBufferData(id);

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

		fillBufferData(id);

		em->addComponent<MaterialComponent>(id);
		em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
		return id;
	}

	unsigned int factoryCylinder(const BasicInfo &info, const glm::vec4 &color) {
		auto id = em->createEntity();
		em->addComponent<Transform>(id);
		systems::transform::updatePosition(id, info.position);
		systems::transform::updateScale(id, info.scale);
		systems::transform::updateRotation(id, info.rotation);
		auto coords = getCylinderVertices();

		auto vc = em->addComponent<VertexComponent>(id, coords.vertex, getColorVector({1, 0, 0, 1}, coords.vertex.size()), coords.indices);
		auto bc = em->addComponent<BufferComponent>(id);
		vc->setNormalsCoords(coords.normals);
		vc->setTexCoords(coords.texCoords);

		fillBufferData(id);

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
		auto pos = glm::vec3{0, -2, 0};
		auto size = glm::vec3{100, 0, 100};
		auto id = factoryCube(BasicInfo{pos, size}, color);
		systems::transform::updateModelMatrix(id);
		em->removeComponent<MaterialComponent>(id);
		auto cc = em->addComponent<ColliderComponent>(id);
		auto pc = em->addComponent<PhysicComponent>(id);
		pc->restitution = 0.4;
		cc->type = ColliderType::COLLIDER_CUBE;
		cc->position = pos;
		cc->normal = {0, 1, 0};
		cc->size = size;
		cc->isStatic = true;
		return id;
	}

	std::vector<std::string> faces{"right", "left", "top", "bottom", "front", "back"};

	unsigned int factorySkyBox(const std::string &path, const std::string &format) {
		auto id = em->createEntity();
		::systems::ecs::updateEntityName(id, "Skybox");
		auto vc = em->addComponent<VertexComponent>(id, skyboxGeometry, std::vector<glm::vec4>{}, std::vector<unsigned int>{});
		auto bc = em->addComponent<BufferComponent>(id);

		// Buffers
		fillBufferData(id);

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
		auto data = readImageData(path, width, height, nrComponents, 0);
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

	/*
	std::vector<ImportedTexture> loadMaterialTextures(const aiMaterial *mat, const aiTextureType &type, const std::string &typeName, const std::string &dir) {
		std::vector<ImportedTexture> textures{};
		auto tt = em->getComponentFromId<ImportedMeshTextures>(mainMesh);
		ASSERT(tt != nullptr);
		flipImagesVertically(false);

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
		if (!info.texCoords.empty()) {
			vc->setTexCoords(info.texCoords);
		}

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

		if (!info.normals.empty()) {
			bc->vbo_n.onAttach();
			bc->vbo_n.setup(vc->getNormalsCoords().data(), vc->getNormalsCoords().size(), GL_STATIC_DRAW);
			bc->vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		}
		if (!info.texCoords.empty()) {
			bc->vbo_t.onAttach();
			bc->vbo_t.setup(vc->getTexCoords().data(), vc->getTexCoords().size(), GL_STATIC_DRAW);
			bc->vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		}

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
		unsigned int id;
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			id = instanceMesh(scene->mMeshes[node->mMeshes[i]], scene, dir);
			systems::parent::addChild(parent, id);
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

		auto sc = em->addComponent<ShaderComponent>(id, LightComputation::PHONG);

		auto dir = pathToFile.substr(0, pathToFile.find_last_of('/'));
		processNode(id, scene->mRootNode, scene, dir);

		return id;
	}
	*/

	const glm::vec3 TREE_LOG_OFFSET = {0.f, -1.f, 0.f};
	const glm::vec3 TREE_LOG_SCALE = {.4f, .7f, .4f};
	const glm::vec3 TREE_LEAF_SCALE = {1, 1.4, 1};
	const glm::vec3 TREE_LEAF_OFFSET = {0, 2 * TREE_LOG_SCALE.y, 0};

	unsigned int factoryTree(const BasicInfo &info) {
		auto id = factoryCylinder(BasicInfo{info.position + TREE_LOG_OFFSET, TREE_LOG_SCALE, {90, 0, 0}});
		em->addComponent<ParentComponent>(id);

		TextureParams params{};
		params.target = GL_TEXTURE_2D;
		params.internalFormat = GL_RGB;
		params.format = GL_RGB;
		params.dataType = GL_UNSIGNED_BYTE;
		int width, height, nrChannels;
		flipImagesVertically(true);
		auto data = readImageData("./resources/texture/wood.jpg", width, height, nrChannels);
		nrChannels = 0;
		ogl::Texture t{params, {(unsigned int)width, (unsigned int)height}};
		t.onAttach();
		t.bind();
		t.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		t.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		t.setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
		t.setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
		t.createTexture2D(data);
		t.generateMipmap();
		em->addComponent<TextureComponent>(id, "./resources/texture/wood.jpg");
		systems::texture::setTexture(id, t);
		freeImageData(data);
		t.unbind();

		auto first = factoryPyramid(BasicInfo{info.position + TREE_LEAF_OFFSET, info.scale * TREE_LEAF_SCALE, {}}, {0, 1, 0, 1});
		em->addComponent<HideTreeComponent>(first);
		systems::parent::addChild(id, first);
		auto ddata = readImageData("./resources/texture/leaves.jpg", width, height, nrChannels);
		ogl::Texture leaves{params, {(unsigned int)width, (unsigned int)height}};
		leaves.onAttach();
		leaves.bind();
		leaves.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		leaves.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		leaves.setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
		leaves.setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
		leaves.createTexture2D(ddata);
		leaves.generateMipmap();
		em->addComponent<TextureComponent>(first, "./resources/texture/leaves.jpg");
		systems::texture::setTexture(first, leaves);
		freeImageData(ddata);
		leaves.unbind();

		auto second = factoryPyramid(BasicInfo{info.position + TREE_LEAF_OFFSET * glm::vec3{0.5}, info.scale * (TREE_LEAF_SCALE + glm::vec3{0.2}), {}}, {0, 1, 0, 1});
		em->addComponent<HideTreeComponent>(second);
		systems::parent::addChild(id, second);
		em->addComponent<TextureComponent>(second);
		systems::texture::setTexture(second, leaves);
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

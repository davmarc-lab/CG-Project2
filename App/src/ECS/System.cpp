#include "../../include/ECS/System.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "../../include/ECS/Component.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/EntityManager.hpp"

const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

const auto BOUNDING_BOX_COLOR = glm::vec4{1, 0, 0, 1};

struct BoundingBox {
	ogl::ShaderProgram program = ogl::ShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
	ogl::VertexArray vao{};
	ogl::VertexBuffer vbog{};
	ogl::VertexBuffer vboc{};
	std::vector<glm::vec3> coords{};
	std::vector<glm::vec4> colors{};
	bool init = false;
} defaultShader;

ogl::ShaderProgram stencil = ogl::ShaderProgram("vertexShader.glsl", "stencilShader.glsl");

namespace systems {
	namespace ecs {
		bool removeEntityFromManager(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					removeEntityFromManager(child);
				}
			}
			return em->removeEntity(id);
		}

		void removeEntityFromScene(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					scene->removeEntity(child);
				}
			}
			scene->removeEntity(id);
		}

		bool removeEntityFromAll(const unsigned int &id) {
			removeEntityFromScene(id);
			return em->removeEntity(id);
		}

		std::string getEntityName(const unsigned int &id) {
			return em->isEntityValid(id) ? em->getEntityName(id) : "INVALID";
		}

		void updateEntityName(const unsigned int &id, const std::string &name) {
			ASSERT(em->isEntityValid(id));
			em->setEntityName(id, name);
		}
	} // namespace ecs
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto offset = cpos - c->getPosition();
					offset.z = 0;
					updatePosition(child, position + offset);
				}
			}

			c->setPosition(position);
		}

		void updateScale(const unsigned int &id, const glm::vec3 &scale) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->setScale(scale);
		}

		void updateRotation(const unsigned int &id, const glm::vec3 &rotation) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					updateRotation(child, rotation);
				}
			}

			c->setRotation(rotation);
		}

		void addPosition(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto modelOffset = cpos - c->getPosition();
					modelOffset.z = 0;
					updatePosition(child, c->getPosition() + modelOffset + offset);
				}
			}

			c->addPosition(offset);
		}

		void addScale(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addScale(offset);
		}

		void addRotation(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addRotation(offset);
		}

		glm::vec3 getPosition(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->position;
		}

		glm::vec3 getScale(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->scale;
		}

		glm::vec3 getRotation(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->rotation;
		}

		glm::mat4 getModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			// ASSERT(tc != nullptr);

			if (tc->isDirty())
				updateModelMatrix(id);
			return tc->model;
		}

		void updateModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			if (tc == nullptr)
				return;

			tc->updateModelMatrix();
			::systems::collision::updateCollider(id);
		}

		void updateAllModelMatrix() {
			for (auto id : em->getEntitiesFromComponent<Transform>()) {
				updateModelMatrix(id);
			}
		}
	} // namespace transform

	namespace collision {
		void updateParentCollider(const unsigned int &id) {
			auto box = em->getComponentFromId<AABB>(id);
			auto tc = em->getComponentFromId<Transform>(id);
			auto offset = tc->position - box->position;
			box->position = tc->position;
			box->botLeft += offset;
			box->topRight += offset;
		}

		void updateCollider(const unsigned int &id) {
			auto bc = em->getComponentFromId<AABB>(id);
			if (bc == nullptr)
				return;

			auto pc = em->getComponentFromId<ParentComponent>(id);
			// has children
			if (pc != nullptr && !pc->children.empty()) {
				updateParentCollider(id);
				return;
			}

			// default collider update
			auto cc = em->getComponentFromId<VertexComponent>(id);
			bc->updateCollider(cc->getVertexCoords(), ::systems::transform::getModelMatrix(id));
		}

		void updateAllColliders() {
			auto ids = em->getEntitiesFromComponent<AABB>();
			for (auto id : em->getEntitiesFromComponent<AABB>()) {
				updateCollider(id);
			}
		}

		bool isColliding(const unsigned int &first, const unsigned int &second) {
			if (first == second)
				return false;
			auto fc = em->getComponentFromId<AABB>(first);
			auto sc = em->getComponentFromId<AABB>(second);

			return fc->isColliding(*sc);
		}

		Pair<glm::vec3> getCollider(const unsigned int &id) {
			auto c = em->getComponentFromId<AABB>(id);
			ASSERT(c != nullptr);

			return {c->botLeft, c->topRight};
		}

		void compressBoundingBox() {
			for (auto id : em->getEntitiesFromComponent<ParentComponent>()) {
				auto c = em->getComponentFromId<ParentComponent>(id);
				auto pv = em->getComponentFromId<VertexComponent>(id);
				auto box = em->getComponentFromId<AABB>(id);
				// remove AABB component from each children and update the parent
				for (auto child : c->children) {
					if (em->entityHasComponent<AABB>(child)) {
						auto cv = em->getComponentFromId<VertexComponent>(child);
						auto model = ::systems::transform::getModelMatrix(child);
						auto bot = glm::vec3(1, 1, 0);
						bool first = true;

						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								bot = elem;
								first = false;
							}
							bot.x = bot.x >= elem.x ? elem.x : bot.x;
							bot.y = bot.y >= elem.y ? elem.y : bot.y;
						}

						auto top = glm::vec3(1, 1, 0);
						first = true;
						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								top = elem;
								first = false;
							}
							top.x = top.x <= elem.x ? elem.x : top.x;
							top.y = top.y <= elem.y ? elem.y : top.y;
						}
						box->botLeft.x = box->botLeft.x < bot.x ? box->botLeft.x : bot.x;
						box->botLeft.y = box->botLeft.y < bot.y ? box->botLeft.y : bot.y;
						box->topRight.x = box->topRight.x > top.x ? box->topRight.x : top.x;
						box->topRight.y = box->topRight.y > top.y ? box->topRight.y : top.y;
						em->removeComponent<AABB>(child);
					}
				}
			}
		}

		void resolveCollisions() {
			auto time = glfwGetTime();
			auto rmv = std::set<unsigned int>{};
			auto colls = getCollisions();
			for (auto c : colls) {
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}

		std::vector<Pair<unsigned int>> getCollisions() {
			std::vector<Pair<unsigned int>> coll{};
			for (auto first : em->getEntitiesFromComponent<AABB>()) {
				for (auto other : em->getEntitiesFromComponent<AABB>()) {
					if (first == other)
						continue;
					{
						auto fc = em->getComponentFromId<ParentComponent>(first);
						if (fc != nullptr) {
							if (std::find(ALL(fc->children), other) != fc->children.end())
								continue;
						}
						auto oc = em->getComponentFromId<ParentComponent>(other);
						if (oc != nullptr) {
							if (std::find(ALL(oc->children), first) != oc->children.end())
								continue;
						}
					}
					if (isColliding(first, other)) {
						if (std::find(ALL(coll), Pair<unsigned int>{first, other}) == coll.end() &&
							std::find(ALL(coll), Pair<unsigned int>{other, first}) == coll.end()) {
							coll.push_back({first, other});
						}
					}
				}
			}
			return coll;
		}

	} // namespace collision

	namespace parent {
		void addChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.push_back(child);
		}

		void removeChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.erase(std::find(ALL(c->children), child));
		}
	} // namespace parent

	namespace input {
		// it can be optimized merging from each InputComponent all callbacks.
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id) {
			// std::cerr << "Optimize this method (read comment above implementation): LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
			std::vector<std::pair<unsigned int, std::function<void()>>> res{};
			auto ic = em->getComponentFromId<InputComponent>(id);
			ASSERT(ic != nullptr);

			for (auto k : ic->getAllKeys()) {
				res.emplace_back(k, ic->callbacks.at(k));
			}
			return res;
		}

		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func) {
			auto ic = em->getComponentFromId<InputComponent>(id);
			if (ic == nullptr)
				return;

			ic->registerAction(key, func);
		}
	} // namespace input

	namespace animation {
		void executeNextFrame(const float &currentTime) {
			std::vector<unsigned int> rmv{};
			for (auto ett : em->getEntitiesFromComponent<TimeAnimation>()) {
				auto c = em->getComponentFromId<TimeAnimation>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(currentTime);
				if (c->dead)
					rmv.push_back(ett);
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}

		void updateDistanceAnimation() {
			std::vector<unsigned int> rmv{};
			/* for (auto ett : em->getEntitiesFromComponent<ProjectileComponent>()) {
				auto c = em->getComponentFromId<ProjectileComponent>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(systems::transform::getPosition(ett));
				if (c->dead)
					rmv.push_back(ett);
			} */
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}
	} // namespace animation

	namespace texture {
		ogl::Texture getTexture(const unsigned int &id) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			return c->texture;
		}

		void setTexture(const unsigned int &id, const ogl::Texture &texture) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);

			c->texture = texture;
		}

		std::string getTexturePath(const unsigned int &id) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			return c->path;
		}

		void updateTexturePath(const unsigned int &id, const std::string &path) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			c->path = path;

			int width, height, nrChannels;
			auto data = readImageData(path, width, height, nrChannels);
			c->texture.setSize({(unsigned int)width, (unsigned int)height});
			c->texture.createTexture2D(data);
			c->texture.unbind();
		}

		bool isMeshReflective(const unsigned int &id) {
			auto c = em->getComponentFromId<ShaderComponent>(id);
			ASSERT(c != nullptr);
			return c->reflective;
		}

		void setMeshReflective(const unsigned int &id, const bool &val) {
			auto c = em->getComponentFromId<ShaderComponent>(id);
			ASSERT(c != nullptr);
			c->reflective = val;
		}

	} // namespace texture

	namespace material {
		Material getMaterial(const unsigned int &id) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			return c->material;
		}

		void updateAmbient(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.ambient = val;
		}

		void updateDiffuse(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.diffuse = val;
		}

		void updateSpecular(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.specular = val;
		}

		void updateShininess(const unsigned int &id, const float &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.shininess = val;
		}

		void updateMaterial(const unsigned int &id, const Material &material) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material = material;
		}

	} // namespace material

	namespace light {
		LightShaderBlock getLightInfo(const unsigned int &id) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			return {
				c->type,
				c->intensity,
				c->color,
				c->position,
				c->direction,
				c->vectors.ambient,
				c->vectors.diffuse,
				c->vectors.specular,
				c->attenuation.constant,
				c->attenuation.linear,
				c->attenuation.quadratic,
				c->cutOff,
				c->outerCutoff};
		}

		void updateColor(const unsigned int &id, const glm::vec3 &color) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->color = color;
		}

		void updateIntensity(const unsigned int &id, const float &intensity) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->intensity = intensity;
		}

		void updateAmbient(const unsigned int &id, const glm::vec3 &ambient) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->vectors.ambient = ambient;
		}

		void updateDiffuse(const unsigned int &id, const glm::vec3 &diffuse) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->vectors.diffuse = diffuse;
		}

		void updateSpecular(const unsigned int &id, const glm::vec3 &specular) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->vectors.specular = specular;
		}

		void updateDirection(const unsigned int &id, const glm::vec3 &direction) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->direction = direction;
		}

		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->position = position;
		}

		void updateConstant(const unsigned int &id, const float &constant) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->attenuation.constant = constant;
		}

		void updateLinear(const unsigned int &id, const float &linear) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->attenuation.linear = linear;
		}

		void updateQuadratic(const unsigned int &id, const float &quadratic) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->attenuation.quadratic = quadratic;
		}

		void updateCutoff(const unsigned int &id, const float &cutoff) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->cutOff = cutoff;
		}

		void updateOuterCutoff(const unsigned int &id, const float &outerCutoff) {
			auto c = em->getComponentFromId<LightComponent>(id);
			ASSERT(c != nullptr);
			c->outerCutoff = outerCutoff;
		}

		LightComputation getLightComputation(const unsigned int &id) {
			auto c = em->getComponentFromId<ShaderComponent>(id);
			ASSERT(c != nullptr);
			return c->computation;
		}
	} // namespace light

	namespace render {
		void initStencilShader() {
			stencil.createShaderProgram();
		}

		void getOtherLightData(LightShaderBlock &block, const Shared<LightComponent> &light) {
			switch (light->type) {
				case LIGHT_DIRECTIONAL: {
					block.direction = light->direction;
					break;
				}
				case LIGHT_POINT: {
					block.position = light->position;
					block.constant = light->attenuation.constant;
					block.linear = light->attenuation.linear;
					block.quadratic = light->attenuation.quadratic;
					break;
				}
				case LIGHT_SPOT: {
					block.position = light->position;
					block.direction = light->direction;
					block.constant = light->attenuation.constant;
					block.linear = light->attenuation.linear;
					block.quadratic = light->attenuation.quadratic;
					block.cutoff = light->cutOff;
					block.outerCutoff = light->outerCutoff;
					break;
				}
				default:
					break;
			}
		}

		std::array<LightShaderBlock, SHADER_MAX_LIGHTS> prepareLightData() {
			size_t index = 0;
			// TODO prepare the light shader block and send data

			std::array<LightShaderBlock, SHADER_MAX_LIGHTS> block{};
			for (auto light : em->getEntitiesFromComponent<LightComponent>()) {
				auto l = em->getComponentFromId<LightComponent>(light);
				block[index].type = l->type;
				block[index].intensity = l->intensity;
				block[index].ambient = l->vectors.ambient;
				block[index].diffuse = l->vectors.diffuse;
				block[index].specular = l->vectors.specular;
				block[index].color = l->color;
				getOtherLightData(block[index], l);
				index++;
			}
			return std::move(block);
		}

		void sendOtherLightData(const Shared<ogl::ShaderProgram> &shader, LightShaderBlock &data, size_t &index) {
			switch (data.type) {
				case LIGHT_DIRECTIONAL: {
					shader->setVec3("lights[" + std::to_string(index) + "].direction", data.direction);
					break;
				}
				case LIGHT_POINT: {
					shader->setVec3("lights[" + std::to_string(index) + "].position", data.position);
					shader->setFloat("lights[" + std::to_string(index) + "].constant", data.constant);
					shader->setFloat("lights[" + std::to_string(index) + "].linear", data.linear);
					shader->setFloat("lights[" + std::to_string(index) + "].quadratic", data.quadratic);
					break;
				}
				case LIGHT_SPOT: {
					shader->setVec3("lights[" + std::to_string(index) + "].position", data.position);
					shader->setVec3("lights[" + std::to_string(index) + "].direction", data.direction);
					shader->setFloat("lights[" + std::to_string(index) + "].constant", data.constant);
					shader->setFloat("lights[" + std::to_string(index) + "].linear", data.linear);
					shader->setFloat("lights[" + std::to_string(index) + "].quadratic", data.quadratic);
					shader->setFloat("lights[" + std::to_string(index) + "].cutOff", data.cutoff);
					shader->setFloat("lights[" + std::to_string(index) + "].outerCutOff", data.outerCutoff);
					break;
				}
				default:
					break;
			}
		}

		void sendLightDataShader(const Shared<ogl::ShaderProgram> &shader, std::array<LightShaderBlock, SHADER_MAX_LIGHTS> data) {
			size_t index = 0;
			for (auto l : data) {
				shader->setInt("lights[" + std::to_string(index) + "].type", l.type);
				shader->setFloat("lights[" + std::to_string(index) + "].intensity", l.intensity);
				shader->setVec3("lights[" + std::to_string(index) + "].ambient", l.ambient);
				shader->setVec3("lights[" + std::to_string(index) + "].diffuse", l.diffuse);
				shader->setVec3("lights[" + std::to_string(index) + "].specular", l.specular);
				shader->setVec3("lights[" + std::to_string(index) + "].color", l.color);

				sendOtherLightData(shader, l, index);
				index++;
			}
			shader->setInt("lightsCount", em->getEntitiesFromComponent<LightComponent>().size());
			shader->setVec3("viewPos", ogl::camera.getCameraPosition());
		}

		void renderSkybox(const unsigned int &id, const Shared<ogl::ShaderProgram> &shader) {
			shader->use();
			auto rc = em->getComponentFromId<RenderComponent>(id);
			shader->setMat4("view", glm::mat4(glm::mat3(ogl::camera.getViewMatrix())));
			shader->setMat4("proj", ogl::camera.getProjMatrix());
			rc->call();
		}

		void renderAllMeshes() {
			auto lightsData = prepareLightData();
			auto sid = em->getEntitiesFromComponent<SkyboxComponent>();
			if (sid.empty())
				return;
			auto skybox = em->getComponentFromId<SkyboxComponent>(sid[0]);
			auto skyboxTexture = em->getComponentFromId<TextureComponent>(sid[0]);
			for (auto [shader, etts] : scene->getShaderEntityMap()) {
				shader->use();
				// send light data
				sendLightDataShader(shader, lightsData);
				for (auto id : etts) {
					shader->setInt("lightComp", ::systems::light::getLightComputation(id));
					shader->setInt("skybox", 1);
					glActiveTexture(GL_TEXTURE1);
					auto mc = em->getComponentFromId<MaterialComponent>(id);
					if (mc != nullptr) {
						shader->setVec3("material.ambient", mc->material.ambient);
						shader->setVec3("material.diffuse", mc->material.diffuse);
						shader->setVec3("material.specular", mc->material.specular);
						shader->setFloat("material.shininess", mc->material.shininess);
					}
					auto sc = em->getComponentFromId<ShaderComponent>(id);
					if (sc != nullptr) {
						if (sc->reflective) {
							skyboxTexture->texture.bind();
							shader->setInt("reflective", 1);
						} else {
							shader->setInt("reflective", 0);
						}
					}
					auto tc = em->getComponentFromId<TextureComponent>(id);
					if (tc != nullptr) {
						glActiveTexture(GL_TEXTURE0);
						shader->setInt("texture1", 0);
						tc->texture.bind();
					}
					auto rc = em->getComponentFromId<RenderComponent>(id);
					if (em->entityHasComponent<Transform>(id)) {
						shader->setMat4("model", ::systems::transform::getModelMatrix(id));
					}
					rc->call();
					if (sc != nullptr && sc->reflective) {
						skyboxTexture->texture.unbind();
					}
					if (tc != nullptr) {
						tc->texture.unbind();
					}
				}
			}
		}

		void renderBoundingBox() {
			if (!defaultShader.init) {
				defaultShader.program.createShaderProgram();
				defaultShader.vao.onAttach();
				defaultShader.vbog.onAttach();
				defaultShader.vboc.onAttach();
				for (int i = 0; i < 8; i++)
					defaultShader.colors.push_back(BOUNDING_BOX_COLOR);
				defaultShader.init = true;
			}

			defaultShader.program.use();
			for (auto ett : em->getEntitiesFromComponent<AABB>()) {
				auto box = em->getComponentFromId<AABB>(ett);
				defaultShader.coords.clear();
				defaultShader.coords.push_back(box->botLeft);
				defaultShader.coords.push_back({box->topRight.x, box->botLeft.y, 0});
				defaultShader.coords.push_back({box->topRight.x, box->botLeft.y, 0});
				defaultShader.coords.push_back(box->topRight);
				defaultShader.coords.push_back(box->topRight);
				defaultShader.coords.push_back({box->botLeft.x, box->topRight.y, 0});
				defaultShader.coords.push_back({box->botLeft.x, box->topRight.y, 0});
				defaultShader.coords.push_back(box->botLeft);

				defaultShader.vao.bind();
				defaultShader.vbog.setup(defaultShader.coords.data(), defaultShader.coords.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				defaultShader.vboc.setup(defaultShader.colors.data(), defaultShader.colors.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

				defaultShader.program.setMat4("model", glm::mat4(1));
				glDrawArrays(GL_LINES, 0, defaultShader.coords.size());
			}
		}
	} // namespace render

} // namespace systems

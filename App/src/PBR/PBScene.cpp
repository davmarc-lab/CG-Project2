#include "../../include/PBR/PBScene.hpp"
#include <glm/geometric.hpp>

#include "../../include/ECS/EntityManager.hpp"
#include "../../include/ECS/System.hpp"

const auto em = EntityManager::instance();
const auto ps = PBScene::instance();

namespace systems {
	namespace pbr {
		void updateMaterial(const unsigned int &id, const PBMaterial &material) {
			auto c = em->getComponentFromId<PBMaterial>(id);
			ASSERT(c != nullptr);
			c->albedo = material.albedo;
			c->roughness = material.roughness;
			c->metallic = material.metallic;
			c->ao = material.ao;
		}

		void updateMaterialAlbedo(const unsigned int &id, const glm::vec3 &albedo) {
			auto c = em->getComponentFromId<PBMaterial>(id);
			ASSERT(c != nullptr);
			c->albedo = glm::normalize(albedo);
		}

		Shared<PBMaterial> getMaterial(const unsigned int &id) {
			auto c = em->getComponentFromId<PBMaterial>(id);
			ASSERT(c != nullptr);
			return c;
		}
	} // namespace pbr

	namespace render {
		void renderScene(const Shared<ogl::ShaderProgram> &shader, ogl::WorldCamera &world) {
			auto lightsData = prepareLightData();
			shader->use();
			sendLightDataShader(shader, lightsData);
			shader->setVec3("camPos", world.camera->getCameraPosition());
			for (auto ett : ps->getEntities()) {
				auto mc = em->getComponentFromId<PBMaterial>(ett);
				if (mc != nullptr) {
					shader->setVec3("material.albedo", mc->albedo);
					shader->setFloat("material.roughness", mc->roughness);
					shader->setFloat("material.metallic", mc->metallic);
					shader->setFloat("material.ao", mc->ao);
				}

				if (em->entityHasComponent<Transform>(ett)) {
					shader->setMat4("model", ::systems::transform::getModelMatrix(ett));
				}

				auto rc = em->getComponentFromId<RenderComponent>(ett);
				if (rc != nullptr) {
					rc->call();
				}
			}
		}
	} // namespace render
} // namespace systems

#include "../include/Factory.hpp"

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <vector>

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

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);

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

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);

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

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);

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

		bc->ebo.onAttach();
		bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

		em->addComponent<MaterialComponent>(id);

		auto rc = em->addComponent<RenderComponent>(id);
		auto vaoid = bc->vao.getId();
		rc->setRenderCall([vc, vaoid]() {
			rd->drawElements(vaoid, GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT);
		});
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

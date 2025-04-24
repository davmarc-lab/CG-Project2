#include "../include/PhysicWorld.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/common.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

const auto em = EntityManager::instance();

std::vector<unsigned int> etts{};

glm::vec3 force{}, acc{}, vel{}, pos{};
float mass{}, dt{};

const float zfighting = 0.01f;

struct CollisionPoints {
	glm::vec3 A{};
	glm::vec3 B{};
	glm::vec3 normal{};
	float depth{};
	bool colliding = false;
};

CollisionPoints testSphereSphere(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	CollisionPoints point{};
	ASSERT(a->type == ColliderType::COLLIDER_SPHERE);
	ASSERT(b->type == ColliderType::COLLIDER_SPHERE);
	glm::vec3 ac = ta->getPosition();
	glm::vec3 bc = tb->getPosition();
	// distance between centers
	auto dir = glm::distance(bc, ac);

	point.normal = (bc - ac) / dir;

	// radius sum
	auto rs = glm::length(point.normal * (ta->getScale() + tb->getScale()));
	point.colliding = dir <= rs;
	point.depth = rs - dir;

	return point;
}

CollisionPoints testPlaneSphere(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	CollisionPoints point{};
	ASSERT(a->type == ColliderType::COLLIDER_CUBE);
	ASSERT(b->type == ColliderType::COLLIDER_SPHERE);

	glm::vec3 ac = ta->getPosition();
	glm::vec3 bc = tb->getPosition();

	// glm::dot((bc - ac), a->normal) = distance vector from sphere center and the plane
	// collPoint = collsion point on the plane
	auto collPoint = bc + (glm::dot((bc - ac), a->normal) * a->normal);

	// direction from sphere center and collision point
	point.normal = glm::normalize(collPoint - bc);
	// distance from sphere center and collision point
	auto dist = glm::distance(bc, collPoint);
	// sphere radius length
	auto rsize = glm::length(tb->getScale() * point.normal) + glm::length(ta->getScale() * point.normal);

	point.depth = rsize - dist;
	point.colliding = dist < rsize;

	return point;
}
CollisionPoints testCubeCube(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	return {};
}

using CollisionFunc = CollisionPoints (*)(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb);

const CollisionFunc testFunc[2][2] = {
	{testSphereSphere, testPlaneSphere},
	{testPlaneSphere, testCubeCube}};

CollisionPoints testCollisions(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	bool swap = b->type > a->type;
	if (swap) {
		std::swap(a, b);
		std::swap(ta, tb);
	}
	auto points = testFunc[a->type][b->type](a, ta, b, tb);

	if (swap) {
		std::swap(points.A, points.B);
		points.normal = -points.normal;
	}
	return points;
}

void CollisionSolver::solve() {
	auto collisions = systems::collision::getCollisions();
	dt = this->world.getWorldDeltaTime();
	etts = this->world.getEntities();

	for (auto [first, second] : collisions) {
		if (first == second)
			continue;

		if (std::find(ALL(etts), first) == etts.end())
			continue;
		if (std::find(ALL(etts), second) == etts.end())
			continue;

		auto ca = em->getComponentFromId<ColliderComponent>(first);
		auto cb = em->getComponentFromId<ColliderComponent>(second);
		if (ca->isStatic && cb->isStatic)
			continue;

		auto ta = em->getComponentFromId<Transform>(first);
		auto tb = em->getComponentFromId<Transform>(second);
		// collision points
		auto p = testCollisions(ca, ta, cb, tb);
		if ((ca->type == COLLIDER_CUBE && cb->type == COLLIDER_SPHERE)) {
			if (ca->isStatic) {
				auto pa = em->getComponentFromId<PhysicComponent>(first);
				auto pb = em->getComponentFromId<PhysicComponent>(second);
				auto bv = systems::physic::getVelocity(second);
				auto speed = glm::dot(bv, p.normal);
				if (speed >= 0) {
					continue;
				}
				auto j = -(1 + (pa->restitution * pb->restitution)) * speed / (1 / pb->mass);
				auto impulse = j * p.normal;
				bv += impulse / pb->mass;
				systems::physic::updateVelocity(second, bv);

				// position solver
				systems::transform::addPosition(second, p.normal * (p.depth / 2));
			}
		}
		if (ca->type == COLLIDER_SPHERE && cb->type == COLLIDER_SPHERE) {
			auto pa = em->getComponentFromId<PhysicComponent>(first);
			auto pb = em->getComponentFromId<PhysicComponent>(first);

			// impulse solver
			auto av = systems::physic::getVelocity(first);
			auto bv = systems::physic::getVelocity(second);
			auto rv = bv - av;
			auto speed = glm::dot(rv, p.normal);
			if (speed >= 0) {
				continue;
			}

			auto j = -(1 + (pa->restitution * pb->restitution)) * speed / ((1 / pa->mass) + (1 / pb->mass));
			auto impulse = j * p.normal;
			av -= impulse / pa->mass;
			bv += impulse / pb->mass;
			systems::physic::updateVelocity(first, av);
			systems::physic::updateVelocity(second, bv);

			// position solver
			systems::transform::addPosition(first, p.normal * (-p.depth / 2));
			systems::transform::addPosition(second, p.normal * (p.depth / 2));
		}
	}
}

void PositionSolver::solve() {
	dt = this->world.getWorldDeltaTime();
	etts = this->world.getEntities();
	for (auto e : etts) {
		systems::transform::addPosition(e, systems::physic::getVelocity(e) * dt);
	}
}

void GravitySolver::solve() {
	if (glfwGetTime() > 1) {
		dt = this->world.getWorldDeltaTime();
		etts = this->world.getEntities();
		for (auto id : etts) {
			if (em->getComponentFromId<ColliderComponent>(id)->isStatic) {
				continue;
			}
			force = systems::physic::getForce(id);
			mass = systems::physic::getMass(id);
			if (force == glm::vec3{0}) {
				force = mass * GRAVITY;
			}
			acc = force / mass;
			systems::physic::updateForce(id, force);
			systems::physic::updateAcceleration(id, acc);
			systems::physic::addVelocity(id, ((force / mass) * dt / glm::vec3(2)));
		}
	}
}

void PhysicWorld::onAttach() {
	this->m_solvers.push_back(CreateShared<GravitySolver>(*this));
	this->m_attached = true;
}

void PhysicWorld::onUpdate() {
	// apply physics
	this->m_currentFrame = glfwGetTime();
	this->m_deltaTime = this->m_currentFrame - this->m_lastFrame;
	this->m_lastFrame = this->m_currentFrame;

	// execute each solver step
	for (auto s : this->m_solvers) {
		s->solve();
	}
}

void PhysicWorld::addEntity(const unsigned int &id) {
	ASSERT(em->entityHasComponent<PhysicComponent>(id));
	ASSERT(em->entityHasComponent<ColliderComponent>(id));

	this->m_entities.push_back(id);
}

bool PhysicWorld::removeEntity(const unsigned int &id) {
	auto elem = std::find(ALL(this->m_entities), id);
	if (elem == this->m_entities.end())
		return false;

	em->removeComponent<PhysicComponent>(id);
	this->m_entities.erase(elem);
	return true;
}

namespace systems {
	namespace physic {
		void resetGravitySolver(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->velocity *= glm::vec3{1, 0, 1};
			c->a = {};
			c->force = {};
		}

		glm::vec3 getVelocity(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			return c->velocity;
		}
		void updateVelocity(const unsigned int &id, const glm::vec3 velocity) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->velocity = velocity;
		}
		void addVelocity(const unsigned int &id, const glm::vec3 offset) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->velocity += offset;
		}

		void updateRestitutionFactor(const unsigned int &id, const float &factor) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->restitution = factor;
		}

		glm::vec3 getAcceleration(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			return c->a;
		}
		void updateAcceleration(const unsigned int &id, const glm::vec3 acceleration) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->a = acceleration;
		}
		void addAcceleration(const unsigned int &id, const glm::vec3 offset) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->a += offset;
		}

		glm::vec3 getForce(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			return c->force;
		}
		void updateForce(const unsigned int &id, const glm::vec3 force) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->force = force;
		}
		void addForce(const unsigned int &id, const glm::vec3 offset) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->force += offset;
		}

		float getMass(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			return c->mass;
		}
		void updateMass(const unsigned int &id, const float &mass) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->mass = mass;
		}
	} // namespace physic
} // namespace systems

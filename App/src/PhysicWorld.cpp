#include "../include/PhysicWorld.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

const auto em = EntityManager::instance();

std::vector<unsigned int> skip{};

glm::vec3 force{}, acc{}, vel{}, pos{};
float mass{};

void CollisionSolver::solve() {
	auto collisions = systems::collision::getCollisions();
	auto etts = this->world.getEntities();

	for (auto [first, second] : collisions) {
		if (first == second)
			continue;

		if (std::find(ALL(etts), first) == etts.end())
			continue;
		if (std::find(ALL(etts), second) == etts.end())
			continue;

		auto fb = systems::collision::getCollider(first);
		auto sb = systems::collision::getCollider(second);
		auto fpos = systems::transform::getPosition(first);
		auto spos = systems::transform::getPosition(second);
		auto offset = (spos - fpos) / glm::vec3{2};
		systems::transform::addPosition(first, -offset);
		systems::transform::addPosition(second, offset);
	}
}

void PositionSolver::solve() {
	auto dt = this->world.getWorldDeltaTime();
	for (auto e : this->world.getEntities()) {
		systems::transform::addPosition(e, systems::physic::getVelocity(e) * dt);
	}
}

void GravitySolver::solve() {
	if (glfwGetTime() > 3) {
		auto dt = this->world.getWorldDeltaTime();
		auto etts = this->world.getEntities();
		for (auto id : etts) {
			if (std::find(ALL(skip), id) != skip.end()) {
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

void PlaneSolver::solve() {
	auto etts = this->world.getEntities();
	int i = 0;
	for (auto id : etts) {
		auto bb = systems::collision::getCollider(id);
		if (bb.x.y < this->m_planePosition.y) {
			skip.push_back(id);
			auto objpos = systems::transform::getPosition(id);
			systems::transform::updatePosition(id, objpos - (bb.x - this->m_planePosition) * glm::vec3(0, 1, 0));
			systems::physic::resetMovement(id);
		} else {
			auto elem = std::find(ALL(skip), id);
			if (elem != skip.end()) {
                skip.erase(elem);
            }
		}
	}
}

void PhysicWorld::onAttach() {
	this->m_solvers.push_back(CreateShared<GravitySolver>(*this));
	this->m_solvers.push_back(CreateShared<CollisionSolver>(*this));
	this->m_attached = true;
}

void PhysicWorld::onUpdate() {
	// apply physics
	this->m_currentFrame = glfwGetTime();
	this->m_deltaTime = this->m_currentFrame - this->m_lastFrame;
	this->m_lastFrame = this->m_currentFrame;

	for (auto s : this->m_solvers) {
		s->solve();
	}
}

void PhysicWorld::addEntity(const unsigned int &id) {
	if (!em->entityHasComponent<PhysicComponent>(id)) {
		em->addComponent<PhysicComponent>(id);
	}
	if (!em->entityHasComponent<ColliderComponent>(id)) {
		em->addComponent<ColliderComponent>(id);
	}
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
		void resetMovement(const unsigned int &id) {
			auto c = em->getComponentFromId<PhysicComponent>(id);
			ASSERT(c != nullptr);

			c->velocity = {};
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

#include "../include/PhysicWorld.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/common.hpp>
#include <glm/ext/quaternion_exponential.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

const auto em = EntityManager::instance();

std::vector<unsigned int> etts{};

glm::vec3 force{}, acc{}, vel{}, pos{};
float mass{}, dt{};

/**
 * @brief Data structure to store collision point in the CollisionSolver and
 * in collision detection phase.
 */
struct CollisionPoints {
	/// first entity collision point
	glm::vec3 A{};
	/// second entity collision point
	glm::vec3 B{};
	/// normalized direction of the collision
	glm::vec3 normal{};
	/// depth of the collision
	float depth{};
	/// tells if the entities are colliding
	bool colliding = false;
};

CollisionPoints testSphereSphere(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	CollisionPoints point{};
	ASSERT(a->type == ColliderType::COLLIDER_SPHERE);
	ASSERT(b->type == ColliderType::COLLIDER_SPHERE);
	glm::vec3 ac = ta->getPosition();
	glm::vec3 bc = tb->getPosition();
	// distance between centers
	auto ds = glm::distance(bc, ac);

	// calculate the normal vector of collision point
	// this defines the direction of the first entity adjustment
	point.normal = (bc - ac) / ds;

	// radius sum
	auto rs = glm::length(point.normal * (ta->getScale() + tb->getScale()));
	// two spheres collides if the distance between centers is less than the sum of their radius
	point.colliding = ds <= rs;
	// how much the spheres are colliding
	// new sphere position = depth * normal
	point.depth = rs - ds;

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
	// sphere radius + plane length
	auto rsize = glm::length(tb->getScale() * point.normal) + glm::length(ta->getScale() * point.normal);

	point.colliding = dist < rsize;
	point.depth = rsize - dist;

	return point;
}

CollisionPoints testCubeCube(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	std::cerr << "Cube to Cube not implemented\n";
	return {};
}

using CollisionFunc = CollisionPoints (*)(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb);

// this matrix contains all function pointers of each test collisions method
// it's indexed using the collider type property of each ColliderComponent
const CollisionFunc testFunc[2][2] = {
	{testSphereSphere, testPlaneSphere},
	{testPlaneSphere, testCubeCube}};

CollisionPoints testCollisions(Shared<ColliderComponent> &a, Shared<Transform> &ta, Shared<ColliderComponent> &b, Shared<Transform> &tb) {
	bool swap = b->type > a->type;
	if (swap) {
		std::swap(a, b);
		std::swap(ta, tb);
	}
	// call the test function stored in the matrix
	auto points = testFunc[a->type][b->type](a, ta, b, tb);

	if (swap) {
		std::swap(points.A, points.B);
		points.normal = -points.normal;
	}
	return points;
}

void CollisionSolver::solve() {
	// get all collisions of the current step
	auto collisions = systems::collision::getCollisions();
	dt = this->world.getWorldDeltaTime();
	etts = this->world.getEntities();

	for (auto [first, second] : collisions) {
		if (first == second)
			continue;

		// if the colliding entity is not in the ECS skip them
		if (std::find(ALL(etts), first) == etts.end())
			continue;
		if (std::find(ALL(etts), second) == etts.end())
			continue;

		// get entities collider
		auto ca = em->getComponentFromId<ColliderComponent>(first);
		auto cb = em->getComponentFromId<ColliderComponent>(second);
		// if both entities are static skip collision response
		if (ca->isStatic && cb->isStatic)
			continue;

		// get transform component where model matrix, position, etc are stored
		auto ta = em->getComponentFromId<Transform>(first);
		auto tb = em->getComponentFromId<Transform>(second);
		// calculate the collision point
		auto p = testCollisions(ca, ta, cb, tb);

		// solve the collision in different ways based from colliders type
		if ((ca->type == COLLIDER_CUBE && cb->type == COLLIDER_SPHERE)) {
			if (ca->isStatic) {
				auto pa = em->getComponentFromId<PhysicComponent>(first);
				auto pb = em->getComponentFromId<PhysicComponent>(second);
				auto bv = pb->velocity;
				// calculate velocity vector
				auto speed = glm::dot(bv, p.normal);
				// avoid attractive sphere
				if (speed >= 0) {
					continue;
				}
				// calculate restitution force
				auto j = -(1 + (pa->restitution * pb->restitution)) * speed / (1 / pb->mass);
				// final force with restitution factor
				auto impulse = j * p.normal;
				// update dynamic mesh velocity
				bv += impulse / pb->mass;
				systems::physic::updateVelocity(second, bv);

				// update position to avoid mesh overlapping
				systems::transform::addPosition(second, p.normal * (p.depth / 2));
			}
		}

		if (ca->type == COLLIDER_SPHERE && cb->type == COLLIDER_SPHERE) {
			auto pa = em->getComponentFromId<PhysicComponent>(first);
			auto pb = em->getComponentFromId<PhysicComponent>(second);

			// impulse solver
			auto av = pa->velocity;
			auto bv = pb->velocity;
			auto rv = bv - av;
			auto speed = glm::dot(rv, p.normal);
			// avoid attractive speed
			if (speed >= 0) {
				continue;
			}

			// calculate restitution force
			auto j = -(1 + (pa->restitution * pb->restitution)) * speed / ((1 / pa->mass) + (1 / pb->mass));
			// final force with restitution factor
			auto impulse = j * p.normal;
			av -= impulse / pa->mass;
			bv += impulse / pb->mass;
			systems::physic::updateVelocity(first, av);
			systems::physic::updateVelocity(second, bv);

			// update position to avoid mesh overlapping
			systems::transform::addPosition(first, p.normal * (-p.depth / 2));
			systems::transform::addPosition(second, p.normal * (p.depth / 2));
		}
	}
}

void PositionSolver::solve() {
	// time between frames
	dt = this->world.getWorldDeltaTime();
	etts = this->world.getEntities();
	// updates all entity position
	for (auto e : etts) {
		systems::transform::addPosition(e, systems::physic::getVelocity(e) * dt);
	}
}

void GravitySolver::solve() {
	if (!this->m_delay) {
		this->m_delay = glfwGetTime() > 1;
	}
	// just wait 1 second before start applying gravity
	if (this->m_delay) {
		dt = this->world.getWorldDeltaTime();
		etts = this->world.getEntities();

		auto colls = em->getEntitiesFromComponent<ColliderComponent>();

		for (auto id : etts) {
			// apply gravity only if the mesh is not static
			if (em->getComponentFromId<ColliderComponent>(id)->isStatic) {
				continue;
			}
			force = mass * GRAVITY;
			mass = systems::physic::getMass(id);
			if (force == glm::vec3{0}) {
			}
			acc = force / mass;
			systems::physic::updateForce(id, force);
			systems::physic::updateAcceleration(id, acc);
			systems::physic::addVelocity(id, ((force / mass) * dt / glm::vec3(2)));
		}
	}
}

// is the given entity a fixed point of a Rope?
bool isInFixed(const std::vector<unsigned int> &elems, const unsigned int &elem) {
	return std::find(ALL(elems), elem) != elems.end();
}

int i, it, i1, i2;
int precision = 20;
void RopeSolver::solve() {
	for (auto rope : em->getEntitiesFromComponent<RopeComponent>()) {
		auto rp = em->getComponentFromId<RopeComponent>(rope);
		auto factor = rp->constant / 2;
		for (it = 0; it < precision; it++) {
			for (i = 1; i < rp->points.size(); i++) {
				i2 = isInFixed(rp->fixedPoints, i);
				i1 = isInFixed(rp->fixedPoints, i - 1);

				// solve rope simulation
				auto p1 = rp->points[i - 1];
				auto p2 = rp->points[i];
				auto pos1 = systems::transform::getPosition(p1);
				auto pos2 = systems::transform::getPosition(p2);

				auto dir = glm::normalize(pos2 - pos1);
				auto err = glm::distance(pos1, pos2) - rp->distance;

				if (!i1 && i2)
					systems::physic::addVelocity(p1, (factor * 2 * dir * err));
				else if (i1 && !i2)
					systems::physic::addVelocity(p2, (-factor * 2 * dir * err));
				else {
					systems::physic::addVelocity(p1, (factor * dir * err));
					systems::physic::addVelocity(p2, (-factor * dir * err));
				}
			}
		}
	}
}

void PhysicWorld::onAttach() {
	// add basic solvers
	this->m_solvers.push_back(CreateShared<GravitySolver>(*this));
	this->m_currentFrame = glfwGetTime();
	this->m_lastFrame = this->m_currentFrame;
	this->m_attached = true;
}

void PhysicWorld::onDetach() {
	this->m_solvers.clear();
	this->m_attached = false;
}

void PhysicWorld::onUpdate() {
	// calculate delta time
	this->m_currentFrame = glfwGetTime();
	this->m_deltaTime = this->m_currentFrame - this->m_lastFrame;
	this->m_lastFrame = this->m_currentFrame;

	// execute each solver step
	std::for_each(ALL(this->m_solvers), [](Shared<Solver> s) { s->solve(); });
}

void PhysicWorld::addEntity(const unsigned int &id) {
	ASSERT(em->entityHasComponent<PhysicComponent>(id));
	ASSERT(em->entityHasComponent<ColliderComponent>(id));

	if (std::find(ALL(this->m_entities), id) == this->m_entities.end())
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

			// this could lead to problems further
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

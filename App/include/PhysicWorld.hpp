#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/ext/vector_float3.hpp>
#include <utility>
#include <vector>

inline glm::vec3 GRAVITY = {0, -9.82f, 0};

class PhysicWorld;

class Solver {
public:
	virtual void solve() { std::cout << "solve\n"; };

	Solver(PhysicWorld &world) :
		world(world) {}

	virtual ~Solver() = default;

	PhysicWorld &world;
};

class PlaneSolver : public Solver {
public:
	virtual void solve() override;

	PlaneSolver(PhysicWorld &world, const glm::vec3 &position) :
		m_planePosition(position), Solver(world) {}

	virtual ~PlaneSolver() override = default;

private:
	glm::vec3 m_planePosition{};
};

class CollisionSolver : public Solver {
public:
    virtual void solve() override;

	CollisionSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~CollisionSolver() override = default;
};

class GravitySolver : public Solver {
public:
	virtual void solve() override;

	GravitySolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~GravitySolver() override = default;
};

class PhysicWorld : public ogl::Layer {
public:
	virtual void onAttach() override;

	virtual void onUpdate() override;

	template <typename T, typename... Args>
	inline void addSolver(Args &&...args) {
		this->m_solvers.push_back(CreateShared<T>(*this, std::forward<Args>(args)...));
	}

	void addEntity(const unsigned int &id);

	bool removeEntity(const unsigned int &id);

	inline float getWorldDeltaTime() const { return this->m_deltaTime; }

	inline std::vector<unsigned int> getEntities() const { return this->m_entities; }

	PhysicWorld(const std::string &name = "Physic World") :
		ogl::Layer(std::move(name)) {}

	virtual ~PhysicWorld() override = default;

private:
	std::vector<unsigned int> m_entities{};
	std::vector<Shared<Solver>> m_solvers{};

	float m_lastFrame = 0;
	float m_currentFrame = 0;
	float m_deltaTime = 0;
};

namespace systems {
	namespace physic {
		void resetMovement(const unsigned int &id);

		glm::vec3 getVelocity(const unsigned int &id);
		void updateVelocity(const unsigned int &id, const glm::vec3 velocity);
		void addVelocity(const unsigned int &id, const glm::vec3 offset);

		glm::vec3 getAcceleration(const unsigned int &id);
		void updateAcceleration(const unsigned int &id, const glm::vec3 acceleration);
		void addAcceleration(const unsigned int &id, const glm::vec3 offset);

		glm::vec3 getForce(const unsigned int &id);
		void updateForce(const unsigned int &id, const glm::vec3 force);
		void addForce(const unsigned int &id, const glm::vec3 offset);

		float getMass(const unsigned int &id);
		void updateMass(const unsigned int &id, const float &mass);
	} // namespace physic
} // namespace systems

#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/ext/vector_float3.hpp>
#include <utility>
#include <vector>

inline glm::vec3 GRAVITY = {0, -9.82f, 0};

class PhysicWorld;

/**
 * @brief A Solver is a
 */
class Solver {
public:
	virtual void solve() { std::cout << "solve\n"; };

	Solver(PhysicWorld &world) :
		world(world) {}

	virtual ~Solver() = default;

	PhysicWorld &world;
};

class CollisionSolver : public Solver {
public:
	virtual void solve() override;

	CollisionSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~CollisionSolver() override = default;
};

class ForceSolver : public Solver {
public:
	virtual void solve() override;

	ForceSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~ForceSolver() override = default;
};

class VelocitySolver : public Solver {
public:
	virtual void solve() override;

	VelocitySolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~VelocitySolver() override = default;
};

class PositionSolver : public Solver {
public:
	virtual void solve() override;

	PositionSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~PositionSolver() override = default;
};

class GravitySolver : public Solver {
public:
	virtual void solve() override;

	GravitySolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~GravitySolver() override = default;
};

/**
 * @brief The `PhysicWorld` is a `Layer` that simulates physic objects
 * in the scene.
 *
 * @warning It needs to be attached, detached, and at every loop iteration
 * should be called `PhysicWorld::onUpdate()`.
 *
 * @note By default it contains some `Solver` to simulate gravity and collision response.
 *
 * @see Solver
 * @see `ogl::Layer`
 */
class PhysicWorld : public ogl::Layer {
public:
	/**
	 * \copydoc ogl::Layer::onAttach()
	 */
	virtual void onAttach() override;

	/**
	 * \copydoc ogl::Layer::onUpdate()
	 */
	virtual void onUpdate() override;

	/**
	 * @brief This method add a `Solver` to physics simulation.
	 *
	 * @tparam T `Solver` solver's class to instance
	 * @tparam Args list of parameters for `Solver` constructor
	 *
	 * @note Everytime a `Solver` is added to the simulation it will create
	 * a shared pointer of type `T`
	 */
	template <typename T, typename... Args>
	inline void addSolver(Args &&...args) {
		this->m_solvers.push_back(CreateShared<T>(*this, std::forward<Args>(args)...));
	}

	/**
	 * @brief Adds the given entity to the simulation.
	 *
	 * @param id entity id
	 */
	void addEntity(const unsigned int &id);

	/**
	 * @brief Removes the given entity from the simulation.
	 *
	 * @param id entity id
	 * @return true if the entity is removed
	 */
	bool removeEntity(const unsigned int &id);

	/**
	 * @brief Removes the given entity from the simulation.
	 *
	 * @return world's delta time
	 */
	inline float getWorldDeltaTime() const { return this->m_deltaTime; }

	/**
	 * Retrieves all the entity that are simulated.
	 *
	 * @return a `std::vector<unsigned int>` containing the entities in the simulation
	 */
	inline std::vector<unsigned int> getEntities() const { return this->m_entities; }

	/**
	 * @brief Construct a simulated world.
	 *
	 * @param name the name of the layer
	 *
	 * @see ogl::Layer
	 */
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
		void resetGravitySolver(const unsigned int &id);

		glm::vec3 getVelocity(const unsigned int &id);
		void updateVelocity(const unsigned int &id, const glm::vec3 velocity);
		void addVelocity(const unsigned int &id, const glm::vec3 offset);

		void updateRestitutionFactor(const unsigned int &id, const float &factor);

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

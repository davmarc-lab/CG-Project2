#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/ext/vector_float3.hpp>
#include <utility>
#include <vector>

inline glm::vec3 GRAVITY = {0, -9.82f, 0};

class PhysicWorld;

/**
 * @brief A Solver defines a generic behaviour in a Physic World.
 *
 * It manipulates the PhysicComponent of an entity during the simulation.
 *
 * A Solver need to be attached to a PhysicWorld to work.
 *
 * @note This class needs to be extended by a specific solver.
 */
class Solver {
public:
	/**
	 * @brief This method is executed at every physic simulation step.
	 *
	 * @note This method must be overrided in the child class.
	 */
	virtual void solve() { std::cout << "solve\n"; };

	/**
	 * @brief Instances basic Solver.
	 *
	 * @param world reference of the PhysicWorld where this solver operates
	 */
	Solver(PhysicWorld &world) :
		world(world) {}

	virtual ~Solver() = default;

	/// the reference to the PhysicWorld
	PhysicWorld &world;
};

/*
 * @brief Solves all collision between entities in the given PhysicWorld.
 */
class CollisionSolver : public Solver {
public:
	/**
	 * @breif It gets all collisions at the current step, and calls a different
	 * method based on the collider type of the entities.
	 * To call the right method it uses the ColliderType as index of a matrix
	 * containing all tests method pointers.
	 *
	 * After testing the collisions, if the entities are colliding it applies
	 * the right modification to his position/physic values.
	 */
	virtual void solve() override;

	CollisionSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~CollisionSolver() override = default;
};

/**
 * @brief It calculates all the entities position from the current velocity.
 */
class PositionSolver : public Solver {
public:
	virtual void solve() override;

	PositionSolver(PhysicWorld &world) :
		Solver(world) {}

	virtual ~PositionSolver() override = default;
};

/**
 * @brief Simulate gravity force towards the negative y world axis.
 */
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
	/// list of entities to be simulated
	std::vector<unsigned int> m_entities{};
	/// list of solvers
	std::vector<Shared<Solver>> m_solvers{};

	/// timestamp in seconds of last frame
	float m_lastFrame = 0;
	/// timestamp in seconds of current frame
	float m_currentFrame = 0;
	/// difference between current and last frame timestamps in seconds
	float m_deltaTime = 0;
};

namespace systems {
	/**
	 * @namespace systems::physic
	 * @brief This namespaces contains all utilities for entities affected
	 * by physic.
	 */
	namespace physic {
		/**
		 * @brief Resets velocity on the on y-axis, acceleration and force
		 * of the given entity.
		 *
		 * @warning This means if there is a velocity on y-axis external to
		 * GravitySolver it will be lost when this method is called.
		 *
		 * @param id entity id
		 */
		void resetGravitySolver(const unsigned int &id);

		/**
		 * @brief Retrieves the velocity of the given entity.
		 *
		 * @param id entity id
		 * @return a glm::vec3 vector containing entity velocity
		 */
		glm::vec3 getVelocity(const unsigned int &id);
		/**
		 * @brief Updates the velocity of the given entity.
		 *
		 * @param id entity id
		 * @param velocity entity new velocity
		 */
		void updateVelocity(const unsigned int &id, const glm::vec3 velocity);
		/**
		 * @brief Adds the given offset to the velocity of the given entity.
		 *
		 * @param id entity id
		 * @param the offset to be added
		 */
		void addVelocity(const unsigned int &id, const glm::vec3 offset);

		/**
		 * @brief Updates the restitution factor of the given entity.
		 *
		 * @param id entity id
		 * @param factor entity new restitution factor
		 */
		void updateRestitutionFactor(const unsigned int &id, const float &factor);

		/**
		 * @brief Retrieves the acceleration of the given entity.
		 *
		 * @param id entity id
		 * @return a glm::vec3 vector containing entity acceleration
		 */
		glm::vec3 getAcceleration(const unsigned int &id);
		/**
		 * @brief Updates the acceleration of the given entity.
		 *
		 * @param id entity id
		 * @param acceleration entity new acceleration
		 */
		void updateAcceleration(const unsigned int &id, const glm::vec3 acceleration);
		/**
		 * @brief Adds the given offset to the acceleration of the given entity.
		 *
		 * @param id entity id
		 * @param the offset to be added
		 */
		void addAcceleration(const unsigned int &id, const glm::vec3 offset);

		/**
		 * @brief Retrieves the force of the given entity.
		 *
		 * @param id entity id
		 * @return a glm::vec3 vector containing entity force
		 */
		glm::vec3 getForce(const unsigned int &id);
		/**
		 * @brief Updates the force of the given entity.
		 *
		 * @param id entity id
		 * @param force entity new force
		 */
		void updateForce(const unsigned int &id, const glm::vec3 force);
		/**
		 * @brief Adds the given offset to the force of the given entity.
		 *
		 * @param id entity id
		 * @param the offset to be added
		 */
		void addForce(const unsigned int &id, const glm::vec3 offset);

		/**
		 * @brief Retrieves the mass of the given entity.
		 *
		 * @param id entity id
		 * @return the entity mass
		 */
		float getMass(const unsigned int &id);
		/**
		 * @brief Updates the mass of the given entity.
		 *
		 * @param id entity id
		 * @param mass entity new mass
		 */
		void updateMass(const unsigned int &id, const float &mass);
	} // namespace physic
} // namespace systems

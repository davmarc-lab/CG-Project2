#pragma once

#include "State.hpp"

#include "../PhysicWorld.hpp"

const ogl::Event RUN_SIMULATION = ogl::Event("Run Simulation");
const ogl::Event STOP_SIMULATION = ogl::Event("Stop Simulation");

/**
 * @brief This State render an interactive scene with rendered meshes and lights.
 *
 * This states simulates physics and collisions between objects.
 * All meshes are rendered using instanced rendering.
 */
class SimulationState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

	/**
	 * @brief Instances basic state with the given name.
	 *
	 * @param name state name
	 */
	SimulationState(const std::string &name = "Simulation State") :
		State(name) {}

	virtual ~SimulationState() override = default;

private:
	/// scene window
	Unique<ogl::Window> m_window;
	/// imgui manager
	Unique<ogl::ImGuiManager> m_img;

	PhysicWorld m_pw{};

	/// scene camera
	ogl::WorldCamera m_world;
	/// scene camera default position
	const glm::vec3 CAMERA_START_POSITION = glm::vec3{0, 1, 3};

	/// uniform buffer for sending data to shaders
	Unique<ogl::UniformBuffer> m_ub;

	/// Defines the default camera behaviour.
	void enableDefaultCameraMovement();

	/// Defines the default key callback function.
	void defaultKeyCallback();
};

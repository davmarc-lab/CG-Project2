#pragma once

#include "State.hpp"

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
	Unique<ogl::Window> m_window;
	Unique<ogl::ImGuiManager> m_img;

	ogl::WorldCamera m_world;
	const glm::vec3 CAMERA_START_POSITION = glm::vec3{0, 1, 3};

	/// uniform buffer for sending data to shaders
	Unique<ogl::UniformBuffer> m_ub;

    void enableDefaultCameraMovement();

    void defaultKeyCallback();
};

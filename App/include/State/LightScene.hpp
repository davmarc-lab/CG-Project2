#pragma once

#include "../../include/Utils.hpp"
#include "State.hpp"

/**
 *
 */
class LightState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

	/**
	 * @brief Instances basic state with the given name.
	 */
	LightState() :
		State("Light Scene") {}

	virtual ~LightState() override = default;

private:
	/// pointer to the window
	Unique<ogl::Window> m_window;
	/// pointer to the imgui manager
	Unique<ogl::ImGuiManager> m_igm;

	/// scene camera
	ogl::WorldCamera m_world;
	/// scene camera default position
	const glm::vec3 CAMERA_START_POSITION = glm::vec3{0, 1, 3};

	/// uniform buffer for sending data to shaders
	Unique<ogl::UniformBuffer> m_ub;
};

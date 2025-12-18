#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

using namespace ogl;

const auto CAMERA_UPDATE_DATA = Event("Camera Update Data");
const auto CAMERA_RESET_POSITION = Event("Camera Reset Position");
const auto NORMAL_VIEW_OPEN = Event("Open Normal View");
const auto NORMAL_VIEW_CLOSE = Event("Close Normal View");

/**
 * @brief Implementation of ImGuiPanel that provides a tree in a panel
 * containing all the entities in the ECS.
 */
class ImGuiEntityTree : public ImGuiPanel {
public:
	virtual void onRender() override;

	/**
	 * @brief Changes the current entity selected by the user.
	 *
	 * @param ett entity id
	 */
	void setSelectedEntity(const int &ett) { this->m_inputEtt = ett; }

	ImGuiEntityTree() :
		ImGuiPanel("Entity Tree") {
	}

	virtual ~ImGuiEntityTree() override = default;

private:
	/// the selected entity
	int m_inputEtt = -1;
	/// the selected material, used for performance
	unsigned int m_selectedMaterial = 0;
};

/**
 * @brief Implementation of ImGuiPanel that provides a window that shows all
 * normal vectors of an entity and draw them on a FrameBuffer.
 */
class ImGuiNormalView : public ImGuiPanel {
public:
	virtual void onRender() override;

	/**
	 * @brief Method that manages the user input in the window, this method
	 * is called at every frame using Layer::onUpdate() functin on
	 * a basic Layer.
	 */
	void processInput();

	ImGuiNormalView();
	virtual ~ImGuiNormalView() override = default;

private:
	/// the framebuffer drawing mesh normals and mesh itself
	Unique<ogl::FrameBuffer> m_fbo;
	/// frambuffer texture
	ogl::Texture m_text;
	/// framebuffer shader program
	Unique<ogl::ShaderProgram> m_shader;
	/// framebuffer normal shader program
	Unique<ogl::ShaderProgram> m_nshader;
	/// framebuffer camera
	Unique<ogl::Camera> m_npcam;

	/// This method resets the camera to its default position.
	void resetCamera();
};

/**
 * @brief Data structure to save simulation configuration.
 */
struct SimulationConfig {
	/// simulation gravity
	glm::vec3 gravity{};
	/// simulation collisions enabled
	bool collisions = true;
	/// simulation is running
	bool run = false;
};

/**
 * @brief Implementation of ImGuiPanel that provides a panel containing
 * all simulation features.
 */
class ImGuiSimulationPanel : public ImGuiPanel {
public:
	virtual void onRender() override;

	/**
	 * @brief Retrieves the configuration of the current physic simulation.
	 *
	 * @return the configuration
	 */
	inline SimulationConfig getConfig() const { return this->m_config; }

	/**
	 * @brief Retrieves the state of the simulation: if it's running or not.
	 *
	 * @return true if the simulation is running
	 */
	inline bool isRunning() const { return this->m_config.run; }
	/**
	 * @brief Sets the state of the simulation.
	 *
	 * @param run if true the simulation run, false the simulation stops
	 */
	inline void setRunning(const bool &run) { this->m_config.run = run; }

	/**
	 * @brief Instances basic simualtion panel.
	 *
	 * @param config the simulation configuration
	 */
	ImGuiSimulationPanel(const SimulationConfig &config) :
		ImGuiPanel("Simulation Panel"), m_config(config) {}

	virtual ~ImGuiSimulationPanel() override = default;

private:
	/// simulation parameters
	SimulationConfig m_config{};
};

/**
 * @brief Implementation of ImGuiPanel, it creates a panel with two buttons Save and Load.
 * When Save is clicked it creates a file called `scene.json` and serialize all entities data.
 * When Load is clicked it loads a file and deserialize all entities data.
 */
class ImGuiMeshLoader : public ImGuiPanel {
public:
	virtual void onRender() override;

	/**
	 * @brief Instances basic panel.
	 */
	ImGuiMeshLoader() :
		ImGuiPanel("Loader Panel") {}

	virtual ~ImGuiMeshLoader() override = default;
};

/**
 * @brief Implementation of ImGuiPanel, it creates a panel that let the user pick one or multiple files.
 * When the Ok button is clicked it execute the action function and closes the panel.
 */
class ImGuiFilePicker : public ImGuiPanel {
public:
	virtual void onRender() override;

	/// Open the panel.
	inline void open() { this->m_open = true; }
	/// Close the panel.
	inline void close() { this->m_open = false; }

	ImGuiFilePicker() = delete;

	/**
	 * @brief Instances basic panel with the given parameters.
	 *
	 * @param path the path to a folder to show
	 * @param extension the file extension to show
	 * @param action the function to execute when Ok button is clicked
	 * @param title the panel title
	 */
	ImGuiFilePicker(const std::string &path, const std::string &extension, std::function<void(const std::string &)> action, const std::string &title = "Pick a file") :
		m_path(path), m_extension(extension), m_action(action), m_title(title), ImGuiPanel("File Picker") {}

private:
	/// panel open/close flag
	bool m_open = false;
	/// panel title
	std::string m_title{};
	/// folder path start point
	std::string m_path{};
	/// file extension
	std::string m_extension{};
	/// action to execute when ok button is clicked
	std::function<void(const std::string &)> m_action;
};

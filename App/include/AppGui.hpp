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

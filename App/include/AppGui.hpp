#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

using namespace ogl;

const auto CAMERA_UPDATE_DATA = Event("Camera Update Data");
const auto CAMERA_RESET_POSITION = Event("Camera Reset Position");
const auto NORMAL_VIEW_OPEN = Event("Open Normal View");
const auto NORMAL_VIEW_CLOSE = Event("Close Normal View");

class ImGuiEntityTree : public ImGuiPanel {
public:
	virtual void onRender() override;

	void setSelectedEntity(const int &ett) { this->m_inputEtt = ett; }

	ImGuiEntityTree() :
		ImGuiPanel("Entity Tree") {
	}

	virtual ~ImGuiEntityTree() override = default;

private:
	int m_inputEtt = -1;
	unsigned int m_selectedMaterial = 0;
};

class ImGuiNormalView : public ImGuiPanel {
public:
	virtual void onRender() override;

	void processInput();

	ImGuiNormalView();
	virtual ~ImGuiNormalView() override = default;

private:
	Unique<ogl::FrameBuffer> m_fbo;
	ogl::Texture m_text;
	Unique<ogl::ShaderProgram> m_shader;
	Unique<ogl::ShaderProgram> m_nshader;
    Unique<ogl::Camera> m_npcam;
};

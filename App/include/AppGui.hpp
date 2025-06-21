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

	ImGuiEntityTree() :
		ImGuiPanel("Entity Tree") {
	}

	virtual ~ImGuiEntityTree() override = default;

private:
	unsigned int m_selectedMaterial = 0;
};

class ImGuiEntityModel : public ImGuiPanel {
public:
	void setSelectedEntity(const int &ett) { this->m_ett = ett; }

	virtual void onRender() override;

	ImGuiEntityModel() :
		ImGuiPanel("Entity Tree") {
	}

	virtual ~ImGuiEntityModel() override = default;

private:
	int m_ett = -1;
};

class ImGuiNormalView : public ImGuiPanel {
public:
	virtual void onRender() override;

	void setInputCallbacks(ogl::Window &w);

	ImGuiNormalView();
	virtual ~ImGuiNormalView() override = default;

private:
	Unique<ogl::FrameBuffer> m_fbo;
	ogl::Texture m_text;
	Unique<ogl::ShaderProgram> m_shader;
	Unique<ogl::Camera> m_npcam;
};

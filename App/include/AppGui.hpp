#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

using namespace ogl;

const auto CAMERA_UPDATE_DATA = Event("Camera Update Data");
const auto CAMERA_RESET_POSITION = Event("Camera Reset Position");

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

class ImGuiNormalView : ImGuiPanel {
public:
	virtual void onRender() override;

	ImGuiNormalView() = default;
	virtual ~ImGuiNormalView() override = default;

private:
};

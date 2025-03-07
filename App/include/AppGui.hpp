#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

using namespace ogl;

class ImGuiEntityTree : public ImGuiPanel {
public:
	virtual void onRender() override;

	ImGuiEntityTree() :
		ImGuiPanel("Entity Tree") {}

	virtual ~ImGuiEntityTree() override = default;
};

class ImGuiEntityModel : public ImGuiPanel {
public:
	void setSelectedEntity(const int &ett) { this->m_ett = ett; }

	virtual void onRender() override;

	ImGuiEntityModel() :
		ImGuiPanel("Entity Tree") {}

	virtual ~ImGuiEntityModel() override = default;

private:
	int m_ett = -1;
};

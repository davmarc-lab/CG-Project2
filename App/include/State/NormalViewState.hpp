#pragma once

#include "../../include/AppGui.hpp"
#include "../../include/State/State.hpp"

class NormalViewState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

	NormalViewState() :
		State("Default State") {}

	virtual ~NormalViewState() override = default;

private:
	struct WorldCamera {
		unsigned int cameraId;
		Shared<Camera> camera;
		glm::vec3 cameraSize = glm::vec3(1);
		int tbBorderTolerance = 20;
		float tbRadius = 1.f;
	} world;

	struct Mouse {
		glm::vec2 pos{};
		bool first = true;
		bool trackState = false;
		bool skipCursorPos = false;
	} mouse;

	double inputWalltime{}, inputCputime{};
	double updateWalltime{}, updateCputime{};
	double renderWalltime{}, renderCputime{};
	std::array<std::reference_wrapper<double>, 3> walltimes{inputWalltime, updateWalltime, renderWalltime};
	std::array<std::reference_wrapper<double>, 3> cputimes{inputCputime, updateCputime, renderCputime};
	std::array<std::string, 3> names{"Input", "Update", "Render"};

	const Event INPUT_NORMAL_VIEW_OPEN = Event("Input in Normal View Start");
	const Event INPUT_NORMAL_VIEW_CLOSE = Event("Input in Normal View Stop");
	const Event ENTITY_ELECTED_CHANGED = Event("Entity Selected Changed");
	const glm::vec3 CAMERA_START_POSITION = glm::vec3{0, 1, 12};

	float lastTime = 0;
	unsigned int plane;

	int ettSelected = -1;
	bool renderBB = false;

	Unique<UniformBuffer> ub;
	// methods
	void enableDefaultCameraMovement();

	glm::vec3 getTrackballPoint(const Pair<float> &viewpSize, const glm::vec2 &pos);

	glm::vec3 getRayFromMouse(const Pair<float> &size, int mouse_x, int mouse_y);

	bool isRayInSphere(const glm::vec3 &ray, const glm::vec3 &sphere_pos, const float &sphere_radius, float *id);

	enum InputState {
		MOUSE_PASSIVE,
		MOUSE_ACTIVE
	};

	void changeInputState(Window *w, const InputState &state);

	void defaultKeyCallback(Window *w);
};

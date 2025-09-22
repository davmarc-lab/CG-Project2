#pragma once

#include "../../include/AppGui.hpp"
#include "State.hpp"
#include "../../include/Utils.hpp"

/**
 * @brief This State render an interactive scene with rendered meshes and lights.
 * The user can modify their internal data, stored in components, thanks to ImGui panels
 * and if the mesh has normals thei can be viewed in a panel using framebuffer for
 * rendering.
 *
 * In this simulation there is no physics but the world camera has a collider so
 * it won't penetrate in meshes inside the scene.
 */
class NormalViewState : public State {
public:
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;
	virtual void onRender() override;

	virtual bool isCurrentStateEnd() override;

	/**
	 * @brief Instances the normal view state.
	 */
	NormalViewState() :
		State("Normal View State") {}

	virtual ~NormalViewState() override = default;

private:
    Mouse mouse;

	/// input wall time
	double inputWalltime{};
	/// input cpu time
	double inputCputime{};
	/// update wall time
	double updateWalltime{};
	/// update cpu time
	double updateCputime{};
	/// render wall time
	double renderWalltime{};
	/// render cpu time
	double renderCputime{};
	/// array with all wall times
	std::array<std::reference_wrapper<double>, 3> walltimes{inputWalltime, updateWalltime, renderWalltime};
	/// array with all cpu times
	std::array<std::reference_wrapper<double>, 3> cputimes{inputCputime, updateCputime, renderCputime};
	/// array used for rendering times
	std::array<std::string, 3> names{"Input", "Update", "Render"};

    ogl::WorldCamera world;

	/// custom Event dispatched when the normal view is opened
	const Event INPUT_NORMAL_VIEW_OPEN = Event("Input in Normal View Start");
	/// custom Event dispatched when the normal view is closed
	const Event INPUT_NORMAL_VIEW_CLOSE = Event("Input in Normal View Stop");
	/// custom Event dispatched whene the selected entity is changed
	const Event ENTITY_SELECTED_CHANGED = Event("Entity Selected Changed");
	const glm::vec3 CAMERA_START_POSITION = glm::vec3{0, 1, 12};

	/// ECS plane id
	unsigned int plane;

	/// ECS id of the selected entity
	int ettSelected = -1;
	/// if true boundig boxes will be rendered
	bool renderBB = false;

	/// uniform buffer for sending data to shaders
	Unique<UniformBuffer> ub;

	/// @brief Defines default camera movement
	void enableDefaultCameraMovement();

	/**
	 * @brief Retrieves the trackball point around a circle.
	 *
	 * @param viewpSize viewport size
	 * @param point the point coords where the ray is pointing
	 */
	glm::vec3 getTrackballPoint(const Pair<float> &viewpSize, const glm::vec2 &pos);

	/**
	 * @brief Retrieves a ray from the center of the camera to the mouse position.
	 *
	 * @param size viewport size
	 * @param mouse_x cursor x coordinate
	 * @param mouse_y cursor y coordinate
	 */
	glm::vec3 getRayFromMouse(const Pair<float> &size, int mouse_x, int mouse_y);

	/**
	 * @brief Return true if the given ray vector is inside a sphere located at
	 * the center of a mesh.
	 *
	 * @param ray a ray from camera center to a point
	 * @param sphere_pos position of the mesh
	 * @param sphere_radius radius of the sphere inside/around a mesh
	 * @param dist pointer to a variable storing the distance between center and meshes
	 *
	 * @return true if the ray is inside the sphere
	 */
	bool isRayInSphere(const glm::vec3 &ray, const glm::vec3 &sphere_pos, const float &sphere_radius, float *dist);

	/**
	 * @brief Changes the current input state.
	 *
	 * @param w pointer to ogl::Window
	 * @param state updated state
	 */
	void changeInputState(Window *w, const InputState &state);

	/**
	 * @brief Sets the default key callback if it's changed.
	 *
	 * @param w pointer to ogl::Window
	 */
	void defaultKeyCallback(Window *w);
};

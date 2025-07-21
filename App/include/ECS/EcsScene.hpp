#pragma once

#include <map>

#include <vector>

#include "../../../Opengl-Core/include/Core.hpp"

/**
 * @brief This class is used to manage a scene with meshes that will be rendered.
 * It's implemented using Singleton.
 */
class BasicScene {
public:
	/**
	 * @brief Initialize the current scene by setting the given world camera.
	 *
	 * The camera will be used to send view and projection data to shaders.
	 *
	 * @param mainCamera the world camera pointer
	 */
	void init(const Shared<ogl::Camera> &mainCamera) { this->mainCamera = mainCamera; }

	/**
	 * @brief Retrieves the world camera shared pointer.
	 *
	 * @return the world ogl::Camera shared pointer
	 */
	Shared<ogl::Camera> getCamera() const { return this->mainCamera; }

	/**
	 * @brief Adds an entity to the scene that will be rendered using the corresponding
	 * shader program.
	 *
	 * This method will store in a map of (ogl::ShaderProgram, std::vector<unsigned int>)
	 * the shader and the entity id. This allow the systems::render::renderAllMesh() method
	 * to call ogl::ShaderProgram::use() and then render all the meshes that uses that program.
	 * This way minimizes the number of calls to enable a shader program at each frame.
	 *
	 * @param shader a ogl::ShaderProgram shared pointer
	 * @param ett an `unsigned int` entity id
	 */
	void addEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);

	/**
	 * @brief Removes the given entity from the scene with the corresponding shader,
	 * to optimize entity search process.
	 *
	 * @param shader a ogl::ShaderProgram shared pointer
	 * @param ett entity id
	 */
	void removeEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);

	/**
	 * @brief Removes the given entity from the scene without knowing the corresponding shader.
	 *
	 * It iterate through all keys and find the entity, if it's in the map it will be erased.
	 *
	 * @param ett entity id
	 */
	void removeEntity(const unsigned int &ett);

	/**
	 * @brief Returns the scene map with key ogl::ShaderProgram shared pointer and a vector
	 * of entity ids (Shared<ogl::Program, std::vector<unsigned int>).
	 *
	 * @return a map containing all the entities for each shader program
	 */
	std::map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> getShaderEntityMap() const { return this->m_entities; }

	BasicScene(BasicScene &other) = delete;

	void operator=(const BasicScene &other) = delete;

	/**
	 * @brief Retrieves the instance of the BasicScene.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<BasicScene> object
	 */
	inline static Shared<BasicScene> instance() {
		if (s_pointer == nullptr) {
			Shared<BasicScene> copy(new BasicScene());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	~BasicScene() = default;

private:
	/// map containing meshes id with their corresponding ogl::ShaderProgram used
	std::map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> m_entities{};

	/// world camera shared pointer
	Shared<ogl::Camera> mainCamera{};

	/// static shared pointer for Singleton
	inline static Shared<BasicScene> s_pointer = nullptr;

	/// @brief Default constructor
	BasicScene() = default;
};

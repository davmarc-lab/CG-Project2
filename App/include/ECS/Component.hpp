#pragma once

#include "../../../Opengl-Core/include/Core.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Utils.hpp"

#include <functional>
#include <glm/trigonometric.hpp>
#include <utility>
#include <vector>

/**
 * @brief Base class for every entity component.
 */
class Component {
public:
	Component() = default;

	virtual ~Component() = default;
};

// REMOVE ALL SET AND GET METHODS - USE SYSTEMS
/**
 * @brief Component to store mesh position, rotation, scale data.
 */
class Transform : public Component {
public:
	/**
	 * @brief Retrieves if the model matrix is enable.
	 *
	 * @return true if model matrix is enabled
	 */
	inline bool isModelMatrixEnable() const { return this->enableModel; }

	/**
	 * @brief Enable/disable the model matrix.
	 *
	 * @param flag `bool` flag to enable or disable model matrix.
	 */
	inline void enableModelMatrix(const bool &flag) { this->enableModel = flag; }

	/**
	 * @brief This method is used to avoid regenerating model matrix if
	 * data aren't changed.
	 *
	 * @return true if data changed and the model matrix is invalid
	 *
	 * @note The model matrix is invalid when position, scale or rotation
	 * are modified.
	 */
	inline const bool &isDirty() const { return this->dirty; }

	/**
	 * @brief Sets the model matrix dirty flag.
	 *
	 * @param dirty model matrix dirty flag
	 *
	 * @note Everytime the dirty flag is activated, model matrix should
	 * be updated.
	 */
	inline void setDirty(const bool &dirty) { this->dirty = true; }

	/* // BETTER DON'T FORCE MODEL MATRIX
	inline void setModelMatrix(const glm::mat4 &matrix) {
		this->model = matrix;
		this->dirty = false;
	}
	*/

	/**
	 * @brief Retrieves the mesh position.
	 *
	 * @return mesh position
	 */
	inline const glm::vec3 &getPosition() const { return this->position; }

	/**
	 * @brief Sets the mesh position.
	 *
	 * @param pos mesh new position
	 *
	 * @note It activate the dirty flag.
	 */
	inline void setPosition(const glm::vec3 &pos) {
		this->position = pos;
		this->dirty = true;
	}

	/**
	 * @brief Adds an offset to the current position.
	 *
	 * @param offset offset to be added.
	 *
	 * @note It activate the dirty flag.
	 */
	inline void addPosition(const glm::vec3 &offset) {
		this->position += offset;
		this->dirty = true;
	}

	/**
	 * @brief Retrieves the mesh position.
	 *
	 * @return mesh position
	 */
	inline glm::vec3 getScale() const { return this->scale; }

	/**
	 * @brief Sets the mesh scale.
	 *
	 * @param scale mesh new scale
	 *
	 * @note It activate the dirty flag.
	 */
	inline void setScale(const glm::vec3 &scale) {
		this->scale = scale;
		this->dirty = true;
	}

	/**
	 * @brief Adds to the current scale the offset given.
	 *
	 * @param offset offset to be added
	 *
	 * @note It activate the dirty flag.
	 */
	inline void addScale(const glm::vec3 &offset) {
		this->scale += offset;
		this->dirty = true;
	}

	/**
	 * @brief Retrieves the mesh position.
	 *
	 * @return mesh position
	 */
	inline glm::vec3 getRotation() const { return this->rotation; }

	/**
	 * @brief Sets the mesh rotation, it works using `MAX_DEGREE_ANGLE`
	 * to avoid angle value more then 360 or less than 0.
	 *
	 * @note It activate the dirty flag.
	 */
	inline void setRotation(glm::vec3 rot) {
		if (rot.x >= MAX_DEGREE_ANGLE || rot.x <= -MAX_DEGREE_ANGLE) {
			rot.x -= 2 * MAX_DEGREE_ANGLE * (rot.x > 0 ? 1.f : -1.f);
		}
		if (rot.y >= MAX_DEGREE_ANGLE || rot.y <= -MAX_DEGREE_ANGLE) {
			rot.y -= 2 * MAX_DEGREE_ANGLE * (rot.y > 0 ? 1.f : -1.f);
		}
		if (rot.z >= MAX_DEGREE_ANGLE || rot.z <= -MAX_DEGREE_ANGLE) {
			rot.z -= 2 * MAX_DEGREE_ANGLE * (rot.z > 0 ? 1.f : -1.f);
		}
		this->rotation = glm::radians(rot);
		this->dirty = true;
	}

	/**
	 * @brief Adds to the current rotation value the given offset.
	 * It works with the same behaviour of `Transform::setRotation(glm::vec3 rot)`.
	 *
	 * @note It activate the dirty flag.
	 */
	inline void addRotation(const glm::vec3 &offset) {
		auto rot = glm::degrees(this->rotation + glm::radians(offset));
		if (rot.x >= MAX_DEGREE_ANGLE || rot.x <= -MAX_DEGREE_ANGLE) {
			rot.x -= 2 * MAX_DEGREE_ANGLE * (rot.x > 0 ? 1.f : -1.f);
		}
		if (rot.y >= MAX_DEGREE_ANGLE || rot.y <= -MAX_DEGREE_ANGLE) {
			rot.y -= 2 * MAX_DEGREE_ANGLE * (rot.y > 0 ? 1.f : -1.f);
		}
		if (rot.z >= MAX_DEGREE_ANGLE || rot.z <= -MAX_DEGREE_ANGLE) {
			rot.z -= 2 * MAX_DEGREE_ANGLE * (rot.z > 0 ? 1.f : -1.f);
		}
		this->rotation = glm::radians(rot);
		this->dirty = true;
	}

	/*
	inline glm::quat getQuaternion() const { return this->quaternion; }

	inline void setQuaternion(const glm::quat &quaternion) {
		this->quaternion = quaternion;
		this->dirty = true;
	}
	*/
	/**
	 * @brief Default constructor.
	 */
	Transform() :
		Component() {
	}

	virtual ~Transform() override = default;

	/**
	 * @brief This method updates the model matrix only if the dirty
	 * flag is activated.
	 */
	void updateModelMatrix() {
		if (!this->dirty)
			return;

		glm::mat4 base{1};
		auto t = glm::translate(base, this->position);
		auto s = glm::scale(base, this->scale);

		this->quaternion = glm::quat(this->rotation);
		auto r = glm::toMat4(this->quaternion);

		this->model = t * s * r;
		this->dirty = false;
	}

	/// mesh position vector
	glm::vec3 position{};
	/// mesh scale
	glm::vec3 scale{1, 1, 1};
	/// mesh rotation
	glm::vec3 rotation{};
	/// mesh quaternion
	glm::quat quaternion{};

	/// mesh model matrix
	glm::mat4 model{1};
	/// model matrix dirty flag
	bool dirty = true;
	/// model matrix enable flag
	bool enableModel = true;

	/**
	 * @note [-180, 180] = 360
	 */
	float MAX_DEGREE_ANGLE = 180;
};

/**
 * @brief Component to store multiple mesh data.
 * This can be used to define an anchor and all the meshes linked will
 * transform using anchor transform data.
 */
class MultiMesh : public Component {
public:
	MultiMesh() :
		Component() {
	}

	virtual ~MultiMesh() override = default;

	/// list of child entities
	std::vector<unsigned int> entities{};
	/// index of the achor entity
	unsigned int anchor = 0;
};

/**
 * @brief Component to store `ogl::Buffer` data.
 *
 * @see ogl::Buffer
 */
class BufferComponent : public Component {
public:
	BufferComponent() = default;

	virtual ~BufferComponent() override = default;

	/// vertex array object
	ogl::VertexArray vao{};
	/// geometry vertex buffer object
	ogl::VertexBuffer vbo_g{};
	/// texture coords vertex buffer object
	ogl::VertexBuffer vbo_t{};
	/// color coords vertex buffer object
	ogl::VertexBuffer vbo_c{};
	/// index element buffer object
	ogl::ElementBuffer ebo{};
	/// normal coords vertex buffer object
	ogl::VertexBuffer vbo_n{};

	/// color offset vertex buffer
	ogl::VertexBuffer vbo_co{};
	/// model matrix offset vertex buffer
	ogl::VertexBuffer vbo_mo{};
};

/**
 * @brief Component to store vertices coords data.
 */
class VertexComponent : public Component {
public:
	/**
	 * @brief Retrieves all mesh vertices coords.
	 *
	 * @return Retrieves a `std::vector<glm::vec3>` vector containing
	 * mesh vertices coords.
	 */
	inline std::vector<glm::vec3> getVertexCoords() const { return this->m_vertex; }

	/**
	 * @brief Sets the vertices coords vector.
	 *
	 * @param vertex a `std::vector<glm::vec3>` containing vertices coords
	 */
	inline void setVertexCoords(const std::vector<glm::vec3> &vertex) { this->m_vertex = vertex; }

	/**
	 * @brief Add at the end of the vertices vector the given vertex.
	 *
	 * @param vertex a `std::vector<glm::vec3>` to be added
	 */
	inline void appendVertex(const std::vector<glm::vec3> &vertex) {
		for (auto e : vertex)
			this->m_vertex.push_back(e);
	}

	/**
	 * @brief Retrieves all mesh normals coords.
	 *
	 * @return Retrieves a `std::vector<glm::vec3>` vector containing
	 * mesh normals coords
	 */
	inline std::vector<glm::vec3> getNormalsCoords() const { return this->m_normals; }

	/**
	 * @brief Sets the vertices coords vector.
	 *
	 * @param normals a `std::vector<glm::vec3>` containing normals coords
	 */
	inline void setNormalsCoords(const std::vector<glm::vec3> &normals) { this->m_normals = normals; }

	/**
	 * @brief Add at the end of the normal vector the given normal.
	 *
	 * @param normal a `std::vector<glm::vec3>` to be added
	 */
	inline void appendNormal(const std::vector<glm::vec3> &normal) {
		for (auto e : normal)
			this->m_normals.push_back(e);
	}

	/**
	 * @brief Retrieves all mesh color values.
	 *
	 * @return Retrieves a `std::vector<glm::vec4>` vector containing
	 * mesh color values
	 */
	inline std::vector<glm::vec4> getColorsCoords() const { return this->m_colors; }

	/**
	 * @brief Sets the colors values vector.
	 *
	 * @param colors a `std::vector<glm::vec4>` containing colors values
	 */
	inline void setColorsCoords(const std::vector<glm::vec4> &colors) { this->m_colors = colors; }

	/**
	 * @brief Add at the end of the colors vector the given colors.
	 *
	 * @param colors a `std::vector<glm::vec4>` to be added
	 */
	inline void appendColor(const std::vector<glm::vec4> &colors) {
		for (auto e : colors)
			this->m_colors.push_back(e);
	}

	/**
	 * @brief Retrieves all mesh texture coords.
	 *
	 * @return Retrieves a `std::vector<glm::vec2>` vector containing
	 * mesh texture coords
	 */
	inline std::vector<glm::vec2> getTexCoords() const { return this->m_texCoords; }

	/**
	 * @brief Sets the texture coords vector.
	 *
	 * @param texCoords a `std::vector<glm::vec2>` containing texture coords
	 */
	inline void setTexCoords(const std::vector<glm::vec2> &texCoords) { this->m_texCoords = texCoords; }

	/**
	 * @brief Add at the end of the texture coords vector the given texture coords.
	 *
	 * @param coords a `std::vector<glm::vec2>` to be added
	 */
	inline void appendTexCoords(const std::vector<glm::vec2> &coords) {
		for (auto e : coords)
			this->m_texCoords.push_back(e);
	}

	/**
	 * @brief Retrieves all mesh indices.
	 *
	 * @return Retrieves a `std::vector<unsigned int>` vector containing
	 * mesh indices for rendering
	 */
	inline std::vector<unsigned int> getIndexCoords() const { return this->m_index; }

	/**
	 * @brief Sets the indices coords vector.
	 *
	 * @param index a `std::vector<unsigned int>` containing indices for render
	 */
	inline void setIndexCoords(const std::vector<unsigned int> &index) { this->m_index = index; }

	/**
	 * @brief Add at the end of the indives vector the given indices.
	 *
	 * @param index a `std::vector<unsigned int>` to be added
	 */
	inline void appendIndex(const std::vector<unsigned int> &index) {
		for (auto e : index)
			this->m_index.push_back(e);
	}

	/**
	 * @brief Instances the VertexComponent object with the given data.
	 *
	 * @param vertex a `std::vector<glm::vec3>` containing vertices coords
	 * @param colors a `std::vector<glm::vec4>` containing colors values
	 * @param indices a `std::vector<unsigned int>` containing indices
	 */
	VertexComponent(const std::vector<glm::vec3> &vertex, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices) :
		Component() {
		this->m_vertex = vertex;
		this->m_colors = colors;
		this->m_index = indices;
	}

	virtual ~VertexComponent() = default;

private:
	// vertices coords vector
	std::vector<glm::vec3> m_vertex{};
	// colors values vector
	std::vector<glm::vec4> m_colors{};
	// texture coords vector
	std::vector<glm::vec2> m_texCoords{};
	// indives vector
	std::vector<unsigned int> m_index{};
	// normals coords vector
	std::vector<glm::vec3> m_normals{};
};

/**
 * @brief Component to store mesh `ogl::Texture` data.
 *
 * @see ogl::Texture
 */
class TextureComponent : public Component {
public:
	/**
	 * @brief Instance an empty Component.
	 */
	TextureComponent() :
		Component() {
	}

	/**
	 * @brief Instance a Component with the given path.
	 *
	 * @params path texture's file path
	 */
	TextureComponent(std::string path) :
		Component(), path(std::move(path)) {
	}

	virtual ~TextureComponent() override = default;

	/// texture object
	ogl::Texture texture{};
	/// texture file path
	std::string path{};
};

/**
 * @brief This Component is used like MultiMesh, but it create a sort of tree
 * structure where each mesh has children. Each children mesh can also
 * have children etc.
 *
 * @note An Entity with MultiMesh component means that each mesh stored
 * in that component follow the anchor transformation. In this component
 * if a children mesh is transformed the parent mesh isn't affected. But
 * if the parent node is transformed all his children are affected.
 */
class ParentComponent : public Component {
public:
	/**
	 * @brief Instance basic Component.
	 */
	ParentComponent() :
		Component() {
	}

	virtual ~ParentComponent() = default;

	/// vector containing children ids.
	std::vector<unsigned int> children{};
};

/**
 * @brief Component to store mesh shader data and some mesh information like
 * light computation and the capability to reflect the skybox texture.
 */
class ShaderComponent : public Component {
public:
	/// Can't use an empty constructor
	ShaderComponent() = delete;

	/**
	 * @brief Instances the Component by giving shaders path and light computation.
	 * The light computation might be used in `systems` while sending data
	 * to shaders.
	 *
	 * @param comp mesh light computation
	 * @param vert vertex shader path
	 * @param frag fragment shader path
	 * @param geom geometry shader path (default none)
	 * @param reflective true if the mesh reflects the skybox (if present)
	 *
	 * @note LightComputation tell to the `ogl::ShaderProgram` which alghoritm
	 * will be used while shading that mesh.
	 */
	ShaderComponent(const LightComputation &comp, const std::string &vert, const std::string &frag, const bool &reflective = false, const std::string &geom = "") :
		computation(comp), vert(std::move(vert)), frag(std::move(vert)), reflective(reflective), geom(std::move(geom)), Component() {}

	virtual ~ShaderComponent() override = default;

	/// mesh light computation @see ShaderComponent(const LightComputation &comp, const std::string &vert, const std::string &frag, const bool &reflective = false, const std::string &geom = "")
	LightComputation computation = LightComputation::PHONG;
	/// mesh reflectiveness
	bool reflective = false;
	/// vertex shader path
	std::string vert{};
	/// fragment shader path
	std::string frag{};
	/// geometry shader path
	std::string geom{};
};

namespace light {
	inline std::vector<LightComputation> lightCompsEnm{NONE, PHONG, BLINN_PHONG, INT_PHONG, INT_BLINN_PHONG};
	inline std::vector<std::string> lightCompStr = {"None", "Phong", "Blinn-Phong", "Int Phong", "Int Blinn-Phong"};
} // namespace light

/**
 * @brief This Component is used to render a mesh in the ogl::Window.
 * It stores the render call that will be called by `systems`.
 */
class RenderComponent : public Component {
public:
	/**
	 * @brief Sets the render call that will be used.
	 *
	 * @param func a callback function
	 */
	void setRenderCall(const std::function<void()> &func) { this->m_renderCall = std::move(func); }

	/**
	 * @brief This method calls the render callback if is defined.
	 */
	void call() {
		if (this->m_renderCall != nullptr)
			this->m_renderCall();
	}

	/**
	 * @brief Instance basic Component.
	 */
	RenderComponent() :
		Component() {
	}

	virtual ~RenderComponent() override = default;

private:
	/// render call function
	std::function<void()> m_renderCall = nullptr;
};

/**
 * @brief This component is used to query the ECS for all primitives that
 * are rendered using instanced rendering.
 */
class InstancedComponent : public Component {
public:
	/**
	 * @brief Instance a Component that will render the given primitive type.
	 *
	 * @params type primitive type to render
	 *
	 * @see ogl::RenderPrimitiveType
	 */
	InstancedComponent(const ogl::RenderPrimitiveType &type) :
		type(type), Component() {}

	virtual ~InstancedComponent() override = default;

	/// primitive type of the mesh
	ogl::RenderPrimitiveType type;
};

/**
 * @brief Component used to query all meshes that needs to be outlined
 * using a simple stencil buffer.
 */
class Outlined : public Component {
public:
	/**
	 * @brief Instances basic Component.
	 */
	Outlined() :
		Component() {
	}

	virtual ~Outlined() = default;
};

/**
 * @brief Basic class to store material information for texturing and light computation.
 */
class Material {
public:
	/// ambient vector
	glm::vec3 ambient{0.f};
	/// diffuse vector
	glm::vec3 diffuse{0.55f};
	/// specular vector
	glm::vec3 specular{0.7f};
	/// material shininess
	float shininess = 32.f;
	/// material name
	std::string name{"New Material"};

	/**
	 * @brief Instances basic material.
	 */
	Material() = default;

	/**
	 * @brief Instances a material with the given data.
	 *
	 * @param ambient a `glm::vec3` vector
	 * @param diffuse a `glm::vec3` vector
	 * @param specular a `glm::vec3` vector
	 * @param shininess material' shininess value
	 * @param name material's name
	 */
	Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const float shininess, const std::string &name) :
		ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), name(std::move(name)) {}

	/**
	 * @brief Overrides the equal operator, two materials are equal if they have
	 * the same name.
	 *
	 * @param other other material
	 */
	bool operator==(const Material &other) {
		return this->name == other.name;
	}
};

/**
 * @namespace material
 *
 * Namespace containing all material utilities.
 */
namespace material {

	/**
	 * @brief In this enum each value is an `unsigned int` so it can be used like index
	 * to select an element from materials pool.
	 */
	enum MaterialType : unsigned int {
		MATERIAL_NONE = 0,
		MATERIAL_RPLASTIC,
		MATERIAL_YPLASTIC,
		MATERIAL_SLATE,
		MATERIAL_BRASS,
		MATERIAL_EMERALD,
	};

	/// initial material types @see MaterialType
	inline std::vector<unsigned int> materialTypes{MATERIAL_NONE, MATERIAL_RPLASTIC, MATERIAL_YPLASTIC, MATERIAL_SLATE, MATERIAL_BRASS, MATERIAL_EMERALD};

	/// vector containing defaults start materials
	inline std::vector<Material> defaultMaterials = {
		Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, "None"),
		Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 32.0f, "Red Plastic"),
		Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.60f, 0.60f, 0.50f), 32.0f, "Yellow Plastic"),
		Material(glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.4f, 0.4f, 0.4f), 1.78125f, "Slate"),
		Material(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f, "Brass"),
		Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f, "Emerald"),
	};

	/**
	 * @brief Retrieves the material by the given type or from a simple
	 * index from the defaultMaterials vector.
	 *
	 * @param index the index or MaterialType
	 *
	 * @see MaterialType
	 */
	inline Material getMaterialFromPool(const MaterialType &index) { return defaultMaterials[index]; }

} // namespace material

/**
 * @brief Component to store the mesh Material data.
 *
 * @see Material
 * @see material::getMaterialFromPool()
 */
class MaterialComponent : public Component {
public:
	/**
	 * @brief Instances a Component with the given material.
	 *
	 * @param material a Material object
	 */
	MaterialComponent(const Material &material = material::defaultMaterials[0]) :
		Component(), material(material) {
	}

	/**
	 * @brief Instances a Component by using the given materila type to retrieve
	 * the Material object.
	 *
	 * @param type a material type or index
	 *
	 * @see material::MaterialType
	 * @see material::getMaterialFromPool()
	 */
	MaterialComponent(const material::MaterialType &type) :
		Component(), material(material::getMaterialFromPool(type)) {
	}

	virtual ~MaterialComponent() = default;

	/// mesh material
	Material material;
};

/**
 * @brief Component to store light data. A light is an emitting mesh, rendered or not,
 * that for each ogl::ShaderProgram used in the scene, its data will be sent to the shader
 */
class LightComponent : public Component {
public:
	LightComponent() = delete;

	/**
	 * @brief Instances a directional light with the given direction.
	 *
	 * @params direction light direction
	 */
	LightComponent(const glm::vec3 &direction) :
		type(LightType::LIGHT_DIRECTIONAL), direction(direction), Component() {
	}

	/**
	 * @brief Instances a point light with the given position and LightConstraint.
	 *
	 * @params position light position
	 * @params constraint light attenuation parameter
	 *
	 * @see LightConstraint
	 */
	LightComponent(const glm::vec3 &position, const LightConstraint &constraint) :
		type(LightType::LIGHT_POINT), position(position), attenuation(constraint), Component() {
	}

	/**
	 * @brief Instances a spot light with the given position, direction,
	 * LightConstraint, light cutoff and outer cutoof.
	 *
	 * @params position light position
	 * @params direction light direction
	 * @params constraint light attenuation parameter
	 * @params cutOff light cutoff parameter
	 * @params outerCutoff light outer cutoff parameter
	 *
	 * @see LightConstraint
	 */
	LightComponent(const glm::vec3 &position, const glm::vec3 &direction, const LightConstraint &constraint, const float &cutOff = 12.5f, const float &outerCutoff = 17.5f) :
		type(LightType::LIGHT_SPOT), position(position), direction(direction), attenuation(constraint), cutOff(cutOff), outerCutoff(outerCutoff), Component() {
	}

	virtual ~LightComponent() override = default;

	/// light color, default white
	glm::vec3 color{1, 1, 1};
	/// light intensity
	float intensity = 1.f;
	/**
     * @brief Light type for shader programs.
     * This member is mainly used to create shader data blocks.
     *
     * @see LightType
     */
	LightType type = LightType::LIGHT_DIRECTIONAL;
	/**
	 * @brief Light components vectors
	 *
	 * @see LightVectors
	 */
	LightVectors vectors{};

	/// light direction
	glm::vec3 direction{1, -1, -1};

	/// light position
	glm::vec3 position{};
	/// light attenuation
	LightConstraint attenuation{};

	/// light cutoff
	float cutOff = 12.5f;
	/// light outer cutoff
	float outerCutoff = 17.5f;

	/// light caster
	bool caster = true;
};

/**
 * @brief Component to store data.
 */
class InputComponent : public Component {
public:
	/**
	 * @brief
	 */
	inline void registerAction(const unsigned int &key, const std::function<void()> &callback) {
		this->callbacks.emplace(key, std::move(callback));
	}

	/**
	 * @brief
	 */
	inline void call(const unsigned int &key) {
		this->callbacks.at(key)();
	}

	/**
	 * @brief
	 */
	inline std::vector<unsigned int> getAllKeys() const {
		std::vector<unsigned int> keys{};
		for (auto [key, _] : this->callbacks) {
			keys.push_back(key);
		}
		return keys;
		// C++20
		// auto kv = std::views::keys(this->callbacks);
		// return std::vector<unsigned int>{kv.begin(), kv.end()};
	}

	InputComponent() = default;

	~InputComponent() override = default;

	std::map<unsigned int, std::function<void()>> callbacks;
};

/**
 * @brief Component to store data.
 */
class TimeAnimation : public Component {
public:
	void updateTick(const float &currentTime) {
		if (this->startTime + this->timeToLive >= currentTime)
			this->func();
		else
			this->dead = true;
	}

	TimeAnimation() = delete;

	TimeAnimation(const float &startTime, const float &timeToLive, std::function<void()> &&func) :
		startTime(startTime), timeToLive(timeToLive), func(func), Component() {
	}

	virtual ~TimeAnimation() override = default;

	std::function<void()> func{};
	float startTime;
	float timeToLive;
	bool dead = false;
};

/**
 * @brief Component to store data.
 */
class BehaviourComponent : public Component {
public:
	BehaviourComponent() :
		Component() {
	}

	virtual ~BehaviourComponent() = default;

	std::function<void()> func = nullptr;
};

/**
 * @brief Component to store data.
 */
class CameraComponent : public Component {
public:
	Shared<ogl::Camera> camera{};

	CameraComponent() = default;
	virtual ~CameraComponent() override = default;
};

/**
 * @brief Component to store data.
 */
class PhysicComponent : public Component {
public:
	glm::vec3 velocity{};
	glm::vec3 a{};
	glm::vec3 force{};

	float mass = 1;
	float restitution = 1;

	PhysicComponent() :
		Component() {}
	virtual ~PhysicComponent() override = default;
};

enum ColliderType : unsigned int {
	COLLIDER_SPHERE = 0,
	COLLIDER_CUBE,
};

/**
 * @brief Component to store data.
 */
class ColliderComponent : public Component {
public:
	ColliderType type = COLLIDER_CUBE;
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec3 size{};
	glm::vec3 botLeft{};
	glm::vec3 topRight{};

	bool isStatic = false;

	// optimized bounding box
	void updateCollider(const std::vector<glm::vec3> &coords, const glm::mat4 &model) {
		auto bot = glm::vec3(0);
		bool first = true;

		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				bot = elem;
				first = false;
			}
			bot.x = bot.x >= elem.x ? elem.x : bot.x;
			bot.y = bot.y >= elem.y ? elem.y : bot.y;
			bot.z = bot.z >= elem.z ? elem.z : bot.z;
		}

		auto top = glm::vec3(1);
		first = true;
		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				top = elem;
				first = false;
			}
			top.x = top.x <= elem.x ? elem.x : top.x;
			top.y = top.y <= elem.y ? elem.y : top.y;
			top.z = top.z <= elem.z ? elem.z : top.z;
		}

		this->botLeft = bot;
		this->topRight = top;
		this->position = model[3];
	}

	// not optimized bounding box
	void updateCollider(const glm::vec3 &position, const glm::vec3 &size) {
		this->botLeft = position - size;
		this->topRight = position + size;
	}

	bool isColliding(const ColliderComponent &other) const {
		return (this->botLeft.x <= other.topRight.x && this->topRight.x >= other.botLeft.x) &&
			(this->botLeft.y <= other.topRight.y && this->topRight.y >= other.botLeft.y) &&
			(this->botLeft.z <= other.topRight.z && this->topRight.z >= other.botLeft.z);
	}

	ColliderComponent() = default;

	ColliderComponent(const glm::vec3 &position, const glm::vec3 &size) {
		this->botLeft = position - size;
		this->topRight = position + size;
		this->position = position;
		this->size = size;
	}

	virtual ~ColliderComponent() override = default;
};

/**
 * @brief Component to store data.
 */
class SkyboxComponent : public Component {
public:
	SkyboxComponent() = delete;

	SkyboxComponent(const unsigned int &textureId) :
		textureId(textureId), Component() {}

	virtual ~SkyboxComponent() override = default;

	unsigned int textureId;
};

struct ImportedTexture {
	unsigned int id;
	std::string type{};
	std::string path{};
};

/**
 * @brief Component to store data.
 */
class ImportedMeshTextures : public Component {
public:
	std::vector<ImportedTexture> textures{};

	ImportedMeshTextures() = default;
	virtual ~ImportedMeshTextures() override = default;
};

/**
 * @brief Component to store data.
 */
class HideTreeComponent : public Component {
public:
	HideTreeComponent() = default;
	virtual ~HideTreeComponent() override = default;
};

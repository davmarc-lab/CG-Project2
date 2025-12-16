#pragma once

#include "../../../Opengl-Core/include/Core.hpp"

#include <glm/gtx/quaternion.hpp>

#include "../Utils.hpp"

#include <functional>
#include <glm/trigonometric.hpp>
#include <json/value.h>
#include <utility>
#include <vector>

using namespace light;

/**
 * @brief Base class for every entity component.
 *
 * A component is basically a structure where some data or callbacks are stored.
 * It's possible to query components using the EntityManager.
 *
 * @see EntityManager
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
class Transform : public Component, public JsonSerializable {
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

	inline virtual Json::Value serialize() override {
		Json::Value elem;
		elem["position"] = seGlm<glm::vec3>(position);
		elem["scale"] = seGlm<glm::vec3>(scale);
		elem["rotation"] = seGlm<glm::vec3>(rotation);
		elem["quaternion"] = seQuat(quaternion);
		elem["model"] = seGlm<glm::mat4>(model);
		elem["dirty"] = dirty;
		elem["enableModel"] = enableModel;
		return elem;
	}

	inline virtual void deserialize(Json::Value &elem) override {
		position = deGlm<glm::vec3>(elem["position"]);
		scale = deGlm<glm::vec3>(elem["scale"]);
		rotation = deGlm<glm::vec3>(elem["rotation"]);
		quaternion = deGlm<glm::quat>(elem["quaternion"]);
		model = deGlm<glm::mat4>(elem["model"]);
		dirty = deVal<bool>(elem["dirty"].asBool());
		enableModel = deVal<bool>(elem["enableModel"].asBool());
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
class MultiMesh : public Component, public JsonSerializable {
public:
	MultiMesh() :
		Component() {
	}

	virtual ~MultiMesh() override = default;

	inline virtual Json::Value serialize() override {
		Json::Value elem;
		// TODO
		// elem["entities"] = seGlm<unsigned int>(entities);
		elem["anchor"] = anchor;
		return elem;
	}

	inline virtual void deserialize(Json::Value &elem) override {
		// TODO
		// position = deGlm<glm::vec3>(elem["position"]);
	}

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
	 * @param path texture's file path
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
 * structure where each mesh has children. Each child mesh can also
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
	/// list of light algorithms for shader programs
	inline std::vector<LightComputation> lightCompsEnm{NONE, PHONG, BLINN_PHONG, INT_PHONG, INT_BLINN_PHONG};
	/// list of strings for each LightComputation
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
	 * @param type primitive type to render
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
	 * @brief Enum for materials declared as `unsigned int`.
	 *
	 * In this enum each value is an `unsigned int` so it can be used like index
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

	/// vector containing default start materials
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
	 * @brief Instances a Component by using the given material type to retrieve
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
	 * @param direction light direction
	 */
	LightComponent(const glm::vec3 &direction) :
		type(LightType::LIGHT_DIRECTIONAL), direction(direction), Component() {
	}

	/**
	 * @brief Instances a point light with the given position and LightConstraint.
	 *
	 * @param position light position
	 * @param constraint light attenuation parameter
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
	 * @param position light position
	 * @param direction light direction
	 * @param constraint light attenuation parameter
	 * @param cutOff light cutoff parameter
	 * @param outerCutoff light outer cutoff parameter
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
 * @brief This Component stores one callback for each input key pressed.
 * When a key is pressed if there are callback linked to that key, the callback
 * will be executed.
 *
 * @note While using with GLFW before calling a callback a `ogl::Window` must
 * be attached.
 */
class InputComponent : public Component {
public:
	/**
	 * @brief Registers a callback to a specific key.
	 *
	 * @param key an `unsigned int` key code
	 * @param callback a function to be called
	 */
	inline void registerAction(const unsigned int &key, const std::function<void()> &callback) {
		this->callbacks.emplace(key, std::move(callback));
	}

	/**
	 * @brief Calls the function in the map at key index.
	 *
	 * @param key an `unsigned int` key code
	 */
	inline void call(const unsigned int &key) {
		this->callbacks.at(key)();
	}

	/**
	 * @brief Retrieves all the key that are linked to a callback.
	 *
	 * @return a `std::vector<unsigned int>` object containing all the key registered
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

	/**
	 * @brief Instances basic Component.
	 */
	InputComponent() = default;

	~InputComponent() override = default;

	/// map containing a callback for a specified key
	std::map<unsigned int, std::function<void()>> callbacks;
};

/**
 * @brief This Component allow to execute an action at every frame for a specified time.
 *
 * @note TimeAnimation::updateTick() must be called at every frame to work.
 */
class TimeAnimation : public Component {
public:
	/**
	 * @brief Updates the current state of the animation by calling the callback.
	 * It uses a `startTime` and a `currentTime` allowing to stop the action after
	 * specified time.
	 *
	 * @param currentTime
	 */
	void updateTick(const float &currentTime) {
		if (this->startTime + this->timeToLive >= currentTime)
			this->func();
		else
			this->dead = true;
	}

	TimeAnimation() = delete;

	/**
	 * @brief Instances an animation that will live `timeToLive` seconds after it has
	 * been created `startTime` and at every frame is needed to execute the callback.
	 *
	 * @param startTime the time when animation starts
	 * @param timeToLive the duration in seconds of the animation
	 * @param func the animation action
	 */
	TimeAnimation(const float &startTime, const float &timeToLive, std::function<void()> &&func) :
		startTime(startTime), timeToLive(timeToLive), func(func), Component() {
	}

	virtual ~TimeAnimation() override = default;

	/// callback function
	std::function<void()> func{};
	/// start time of the animation
	float startTime;
	/// duration of the animation
	float timeToLive;
	/// determines if the animation is finished, if it's true the callback will be called
	bool dead = false;
};

/**
 * @brief This Component is very similar to TimeAnimation, the only difference is that
 * this "animation" has not an end but it defines a behaviour of a mesh (like an AI).
 */
class BehaviourComponent : public Component {
public:
	/**
	 * @brief Instances basic Component to query the ECS when it needs to update
	 * a mesh position or color or scale etc.
	 */
	BehaviourComponent() :
		Component() {
	}

	virtual ~BehaviourComponent() = default;

	/// function called every frame to update the mesh
	std::function<void()> func = nullptr;
};

/**
 * @brief Component to store data of a ogl::Camera.
 *
 * It allows the ECS to interact with the camera in the scene.
 * For example it can be showed in an ImGuiPanel and the user can modify
 * the internal parameters from there.
 */
class CameraComponent : public Component {
public:
	/// shared pointer to the ogl::Camera object
	Shared<ogl::Camera> camera{};

	/**
	 * @brief Instances basic Component to query the ECS when needed.
	 */
	CameraComponent() = default;
	virtual ~CameraComponent() override = default;
};

/**
 * @brief This Component stores all the mesh data regarding basic physics like
 * velocity, force, acceleration.
 *
 * All the meshes with this Component must be added to a PhysicWorld to simulate
 * the physics.
 *
 * @see PhysicWorld
 */
class PhysicComponent : public Component {
public:
	/// mesh velocity
	glm::vec3 velocity{};
	/// mesh acceleration
	glm::vec3 a{};
	/// mesh force
	glm::vec3 force{};

	/// mesh mass
	float mass = 1;
	/// mesh restitution factor
	float restitution = 1;

	/**
	 * @brief Instances basic Component to query the ECS.
	 */
	PhysicComponent() :
		Component() {}

	virtual ~PhysicComponent() override = default;
};

/**
 * @brief This Component stores all information to manage a rope in the PhysicWorld.
 */
class RopeComponent : public Component {
public:
	/// number of subdivisions
	unsigned int subdivisions = 3;
	/// position in the world of the center
	glm::vec3 center{};
	/// rope length
	float length{};
	/// rope constant value
	float constant = 1.f;
	/// rope list of points, each value is an id of an entity in the ECS
	std::vector<unsigned int> points{};
	/// rope list of fixed points (points not affected by physics or held bye something)
	std::vector<unsigned int> fixedPoints{};
	/// distance from each point (length / subdivisions)
	float distance;

	/**
	 * @brief Instances a basic rope.
	 *
	 * @param center the rope center point
	 * @param length the rope total length
	 * @param sub the rope subdivisions value
	 * @param constant the rope constant value
	 */
	RopeComponent(const glm::vec3 &center, const float &length, const unsigned int &sub, const float &constant) :
		center(center), length(length), subdivisions(sub), constant(constant), distance(length / subdivisions), Component() {}

	virtual ~RopeComponent() override = default;
};

// TODO: put it in a namespace
/**
 * @enum ColliderType
 * @brief Types of collider to handle collision response in a PhysicWorld.
 *
 * @note All kind of collision response in a PhysicWorld are defined
 * in Solvers and after they need to be attached to the PhysicWorld.
 *
 * @see Solver
 */
enum ColliderType : unsigned int {
	COLLIDER_SPHERE = 0,
	COLLIDER_CUBE,
};

/**
 * @brief Component to store mesh collider data to solve collision in a PhysicWorld.
 *
 * @note It is also possible to use a default system of collision detection
 * integrated with ECS systems, it uses a simple AABB detection.
 *
 * @see systems::collision::getCollisions()
 */
class ColliderComponent : public Component {
public:
	/// collider type
	ColliderType type = COLLIDER_CUBE;
	/// collider center position
	glm::vec3 position{};
	/// collider normal vector (in case is a plane)
	glm::vec3 normal{};
	/// collider size
	/// @note It need to follow the projection: in case the projection is ortho
	/// the size should be different from perspective projection.
	glm::vec3 size{};
	/// bottom left corner coords
	glm::vec3 botLeft{};
	/// top right corner coords
	glm::vec3 topRight{};

	/// define if the mesh is simulated in the PhsyicWorld or not
	bool isStatic = false;

	/**
	 * @brief This method updates the collider cornders coords by iterating over all
	 * mesh vertices.
	 * It will find the minimum and the maximum corner in World Space
	 * Coordinates, the corresponding vertex coords are the new bottom left and
	 * top right corners.
	 *
	 * @note This is also called in this project: Optimized Bounding Box.
	 * This means that the collider "perfectly" contains the mesh in the most
	 * efficient single square.
	 */
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

	/**
	 * @brief This method update a non Optimized Bounding Box by summing the given size
	 * with the given position.
	 * This kind of update is generally used for colliders where doesn't matter
	 * the precision of collision detection.
	 *
	 * @param position collider center position
	 * @param size collider size
	 *
	 * @see ColliderComponent::size
	 */
	void updateCollider(const glm::vec3 &position, const glm::vec3 &size) {
		this->botLeft = position - size;
		this->topRight = position + size;
	}

	/**
	 * @brief Checks if the given collider is overlapping with the current one.
	 *
	 * @param other collider of the other mesh
	 *
	 * @return true if other and this colliders are overlapping
	 */
	bool isColliding(const ColliderComponent &other) const {
		return (this->botLeft.x <= other.topRight.x && this->topRight.x >= other.botLeft.x) &&
			(this->botLeft.y <= other.topRight.y && this->topRight.y >= other.botLeft.y) &&
			(this->botLeft.z <= other.topRight.z && this->topRight.z >= other.botLeft.z);
	}

	/**
	 * @brief Instances a basic Component.
	 */
	ColliderComponent() = default;

	/**
	 * @brief Instances a non Optimized Bounding Box collider.
	 *
	 * @param position collider center position
	 * @param size collider size (center-right)
	 */
	ColliderComponent(const glm::vec3 &position, const glm::vec3 &size) {
		this->botLeft = position - size;
		this->topRight = position + size;
		this->position = position;
		this->size = size;
	}

	virtual ~ColliderComponent() override = default;
};

/**
 * @brief Component to store SkyBox texture data.
 *
 * It's only purpos is to keep track in the ECS of the Skybox texture id.
 * It coul be implemented some methods to change the skybox texture by
 * changing the texture id and clean the previous one.
 *
 * @see ogl::Texture
 */
class SkyboxComponent : public Component {
public:
	SkyboxComponent() = delete;

	/**
	 * @brief Instances basic Component and stores the texture id of the skybox.
	 *
	 * @param textureId the skybox texture id
	 */
	SkyboxComponent(const unsigned int &textureId) :
		textureId(textureId), Component() {}

	virtual ~SkyboxComponent() override = default;

	/// skybox texture id
	unsigned int textureId;
};

/**
 * @brief This structure stores data from an imported mesh texture.
 */
struct ImportedTexture {
	/// texture id
	unsigned int id;
	/// texture type (diffuse, normal, specular, etc.)
	std::string type{};
	/// texture file path
	std::string path{};
};

/**
 * @brief Component to store assimp imported mesh data.
 */
class ImportedMeshTextures : public Component {
public:
	/// imported texture list of the mesh
	std::vector<ImportedTexture> textures{};

	/**
	 * @brief Instances basic Component to query the ECS.
	 */
	ImportedMeshTextures() = default;
	virtual ~ImportedMeshTextures() override = default;
};

/**
 * @brief This Component allow to hide some entity while rendering the
 * ImGuiEntityTree panel.
 *
 * @see ImGuiEntityTree
 */
class HideTreeComponent : public Component {
public:
	/**
	 * @brief Instances basic Component to query the ECS.
	 */
	HideTreeComponent() = default;
	virtual ~HideTreeComponent() override = default;
};

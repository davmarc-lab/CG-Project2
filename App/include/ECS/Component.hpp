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

class Component {
public:
	Component() = default;

	virtual ~Component() = default;
};

// REMOVE ALL SET AND GET METHODS - USE SYSTEMS
class Transform : public Component {
public:
	inline bool isModelMatrixEnable() const { return this->enableModel; }

	inline void enableModelMatrix(const bool &flag) { this->enableModel = flag; }

	inline const bool &isDirty() const { return this->dirty; }

	inline void setDirty(const bool &dirty) { this->dirty = true; }

	/* // BETTER DON'T FORCE MODEL MATRIX
	inline void setModelMatrix(const glm::mat4 &matrix) {
		this->model = matrix;
		this->dirty = false;
	}
	*/

	inline const glm::vec3 &getPosition() const { return this->position; }

	inline void setPosition(const glm::vec3 &pos) {
		this->position = pos;
		this->dirty = true;
	}

	inline void addPosition(const glm::vec3 &offset) {
		this->position += offset;
		this->dirty = true;
	}

	inline glm::vec3 getScale() const { return this->scale; }

	inline void setScale(const glm::vec3 &scale) {
		this->scale = scale;
		this->dirty = true;
	}

	inline void addScale(const glm::vec3 &offset) {
		this->scale += offset;
		this->dirty = true;
	}

	inline glm::vec3 getRotation() const { return this->rotation; }

	inline void setRotation(const glm::vec3 &rotation) {
		this->rotation = glm::radians(rotation);
		this->dirty = true;
	}

	inline void addRotation(const glm::vec3 &offset) {
		this->rotation += glm::radians(offset);
		this->dirty = true;
	}

	/*
	inline glm::quat getQuaternion() const { return this->quaternion; }

	inline void setQuaternion(const glm::quat &quaternion) {
		this->quaternion = quaternion;
		this->dirty = true;
	}
	*/
	Transform() :
		Component() {
	}

	virtual ~Transform() override = default;

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

	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
	glm::quat quaternion{};

	glm::mat4 model{1};
	bool dirty = true;
	bool enableModel = true;
};

class MultiMesh : public Component {
public:
	MultiMesh() :
		Component() {}

	virtual ~MultiMesh() override = default;

	std::vector<unsigned int> entities{};
	unsigned int anchor = 0;
};

class BufferComponent : public Component {
public:
	BufferComponent() = default;

	virtual ~BufferComponent() override = default;

	ogl::VertexArray vao{};
	ogl::VertexBuffer vbo_g{};
	ogl::VertexBuffer vbo_t{};
	ogl::VertexBuffer vbo_c{};
	ogl::ElementBuffer ebo{};
	ogl::VertexBuffer vbo_n{};
};

class VertexComponent : public Component {
public:
	inline std::vector<glm::vec3> getVertexCoords() const { return this->m_vertex; }
	inline void setVertexCoords(const std::vector<glm::vec3> &vertex) { this->m_vertex = vertex; }

	inline void appendVertex(const std::vector<glm::vec3> &vertex) {
		for (auto e : vertex)
			this->m_vertex.push_back(e);
	}

	inline std::vector<glm::vec3> getNormalsCoords() const { return this->m_normals; }
	inline void setNormalsCoords(const std::vector<glm::vec3> &normals) { this->m_normals = normals; }

	inline void appendNormal(const std::vector<glm::vec3> &normal) {
		for (auto e : normal)
			this->m_normals.push_back(e);
	}

	inline std::vector<glm::vec4> getColorsCoords() const { return this->m_colors; }
	inline void setColorsCoords(const std::vector<glm::vec4> &colors) { this->m_colors = colors; }

	inline void appendColor(const std::vector<glm::vec4> &colors) {
		for (auto e : colors)
			this->m_colors.push_back(e);
	}

	inline std::vector<unsigned int> getIndexCoords() const { return this->m_index; }
	inline void setIndexCoords(const std::vector<unsigned int> &index) { this->m_index = index; }

	inline void appendIndex(const std::vector<unsigned int> &index) {
		for (auto e : index)
			this->m_index.push_back(e);
	}

	VertexComponent(const std::vector<glm::vec3> &vertex, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices) :
		Component() {
		this->m_vertex = vertex;
		this->m_colors = colors;
		this->m_index = indices;
	}

	virtual ~VertexComponent() = default;

private:
	std::vector<glm::vec3> m_vertex{};
	std::vector<glm::vec4> m_colors{};
	std::vector<unsigned int> m_index{};
	std::vector<glm::vec3> m_normals{};
};

class TextureComponent : public Component {
public:
	TextureComponent() :
		Component() {};

    virtual ~TextureComponent() = default;

    std::vector<ogl::Texture> textures{};
};

class ParentComponent : public Component {
public:
	ParentComponent() :
		Component() {}

	virtual ~ParentComponent() = default;

	std::vector<unsigned int> children{};
};

class ShaderComponent : public Component {
public:
	ShaderComponent() :
		Component() {
	}

	virtual ~ShaderComponent() override = default;

private:
	Shared<ogl::ShaderProgram> m_shader{};
};

class RenderComponent : public Component {
public:
	void setRenderCall(const std::function<void()> &func) { this->m_renderCall = std::move(func); }

	void call() {
		if (this->m_renderCall != nullptr)
			this->m_renderCall();
	}

	RenderComponent() :
		Component() {
	}

	virtual ~RenderComponent() override = default;

private:
	std::function<void()> m_renderCall = nullptr;
};

class Outlined : public Component {
public:
	Outlined() :
		Component() {}

	virtual ~Outlined() = default;
};

struct Material {
	glm::vec3 ambient{0.f};
	glm::vec3 diffuse{0.55f};
	glm::vec3 specular{0.7f};
	float shininess = 32.f;
};

class MaterialComponent : public Component {
public:
	MaterialComponent() :
		Component() {}

	virtual ~MaterialComponent() = default;

	Material material{};
};

class LightComponent : public Component {
public:
	LightComponent() = delete;

	// Directional Light
	LightComponent(const glm::vec3 &direction) :
		type(LightType::LIGHT_DIRECTIONAL), direction(direction), Component() {}

	// Point Light
	LightComponent(const glm::vec3 &position, const LightConstraint &constraint) :
		type(LightType::LIGHT_POINT), position(position), attenuation(constraint), Component() {}

	// Spot Light
	LightComponent(const glm::vec3 &position, const glm::vec3 &direction, const LightConstraint &constraint, const float &cutOff = 12.5f, const float &outerCutoff = 17.5f) :
		type(LightType::LIGHT_SPOT), position(position), direction(direction), attenuation(constraint), cutOff(cutOff), outerCutoff(outerCutoff), Component() {}

	virtual ~LightComponent() override = default;

	glm::vec3 color{1, 1, 1};
	float intensity = 1.f;
	LightType type = LightType::LIGHT_DIRECTIONAL;
	LightVectors vectors{};

	glm::vec3 direction{1, -1, -1};

	glm::vec3 position{};
	LightConstraint attenuation{};

	float cutOff = 12.5f, outerCutoff = 17.5f;

	bool caster = true;
};

class InputComponent : public Component {
public:
	inline void registerAction(const unsigned int &key, const std::function<void()> &callback) {
		this->callbacks.emplace(key, std::move(callback));
	}

	inline void call(const unsigned int &key) {
		this->callbacks.at(key)();
	}

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
		startTime(startTime), timeToLive(timeToLive), func(func), Component() {}

	virtual ~TimeAnimation() override = default;

	std::function<void()> func{};
	float startTime;
	float timeToLive;
	bool dead = false;
};

class BehaviourComponent : public Component {
public:
	BehaviourComponent() :
		Component() {}

	virtual ~BehaviourComponent() = default;

	std::function<void()> func = nullptr;
};

class AABB : public Component {
public:
	glm::vec3 position{};
	glm::vec3 botLeft{};
	glm::vec3 topRight{};

	void updateCollider(const std::vector<glm::vec3> &coords, const glm::mat4 &model) {
		auto bot = glm::vec3(1, 1, 0);
		bool first = true;

		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				bot = elem;
				first = false;
			}
			bot.x = bot.x >= elem.x ? elem.x : bot.x;
			bot.y = bot.y >= elem.y ? elem.y : bot.y;
		}

		auto top = glm::vec3(1, 1, 0);
		first = true;
		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				top = elem;
				first = false;
			}
			top.x = top.x <= elem.x ? elem.x : top.x;
			top.y = top.y <= elem.y ? elem.y : top.y;
		}

		this->botLeft = bot;
		this->topRight = top;
		this->position = model[3];
	}

	bool isColliding(const AABB &other) const {
		return (this->botLeft.x <= other.topRight.x && this->topRight.x >= other.botLeft.x) &&
			(this->botLeft.y <= other.topRight.y && this->topRight.y >= other.botLeft.y);
	}

	AABB() = default;

	virtual ~AABB() override = default;
};

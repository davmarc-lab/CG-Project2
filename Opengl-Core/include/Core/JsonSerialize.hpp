#include "Serialize.hpp"

#include "Utils.hpp"

#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <json/json.h>

#include <type_traits>
#include <vector>

/**
 * @brief This class provides overridable methods to serialize and deserialize a class in Json data.
 */
class JsonSerializable : public Serializable<Json::Value> {
public:
	virtual ~JsonSerializable() = default;

	JsonSerializable() = delete;
	/**
	 * @brief Instances basic serializable object, it will use the given name as root element.
	 *
	 * @param name the json root element key
	 */
	JsonSerializable(const std::string &name) : m_name(std::move(name)) {}

	virtual Json::Value serialize() override { return Json::nullValue; }

	virtual void deserialize(Json::Value &elem) override {}

	/**
	 * @brief Retrieves the element name.
	 *
	 * @return a string containing the element name
	 */
	inline std::string getName() const { return this->m_name; }

	// template specialization to distinguish glm objects
	template <typename T>
	struct is_glm_type : std::false_type {};

	template <glm::length_t L, typename T, glm::qualifier Q>
	struct is_glm_type<glm::vec<L, T, Q>> : std::true_type {};

	template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	struct is_glm_type<glm::mat<C, R, T, Q>> : std::true_type {};

	template <typename T, glm::qualifier Q>
	struct is_glm_type<glm::qua<T, Q>> : std::true_type {};

	/**
	 * @brief Serializes an `std::vector` containing glm values.
	 *
	 * @param vec the vector to serialize
	 * @tparam T the glm element type inside the vector
	 *
	 * @return a Json::Value element containing the vec data
	 */
	template <typename T>
	inline static Json::Value seGlmVec(std::vector<T> vec) {
		Json::Value arr(Json::arrayValue);
		if (is_glm_type<T>::value)
			for (auto e : vec) {
				arr.append(seGlm(e));
			}
		return arr;
	}

	/**
	 * @brief Serializes an `std::vector` containing standard values.
	 *
	 * @param vec the vector to serialize
	 * @tparam T the standard element type inside the vector
	 *
	 * @return a Json::Value element containing the vec data
	 */
	template <typename T>
	inline static Json::Value seVec(std::vector<T> vec) {
		Json::Value arr(Json::arrayValue);
		for (auto e : vec)
			arr.append(e);
		return arr;
	}

	/**
	 * @brief Serializes a glm element.
	 *
	 * @param glm the element to serialize
	 * @tparam T the glm element type
	 *
	 * @return a Json::Value element containing the glm data
	 */
	template <typename T>
	inline static Json::Value seGlm(T &glm) {
		// glm packed value type
		using TypeT = typename T::value_type;
		// number of packed elements
		const size_t n = sizeof(T);

		TypeT data[n];
		std::memcpy(data, glm::value_ptr(glm), n);

		Json::Value arr(Json::arrayValue);
		for (size_t i = 0; i < n / sizeof(TypeT); i++)
			arr.append(data[i]);
		return arr;
	}

	/**
	 * @brief Serializes a glm quaternion.
	 *
	 * @param q the glm quat to serialize
	 * @tparam T the glm element inside the vector
	 *
	 * @warning It can be merged with seGlm
	 *
	 * @return a Json::Value element containing the vec data
	 */
	inline Json::Value seQuat(glm::quat &q) {
		auto vec = glm::vec4(q.x, q.y, q.z, q.w);
		return seGlm<glm::vec4>(vec);
	}

	/**
	 * @brief Deserializes all standard type elements inside a std::vector.
	 *
	 * @param arr the Json::Value array
	 * @tparam T the standard type to deserialize each element
	 *
	 * @return a vector containing all deserialized elements
	 */
	template <typename T>
	static std::vector<T> deVec(Json::Value &arr) {
		if (!arr.isArray()) {
			return {};
		}

		std::vector<T> data{};
		for (auto e : arr) {
			data.push_back(e.as<T>());
		}

		return data;
	}

	/**
	 * @brief Deserializes all the glm type elements inside a std::vector.
	 *
	 * @param arr the Json::Value array
	 * @tparam T the glm type to deserialize each element
	 *
	 * @return a vector containing all deserialized elements
	 */
	template <typename T>
	inline static std::vector<T> deGlmVec(Json::Value &arr) {
		std::vector<T> data{};
		if (arr.isArray())
			for (auto e : arr) {
				data.push_back(deGlm<T>(e));
			}
		return data;
	}

	/**
	 * @brief Deserializes a glm element.
	 *
	 * @param glm the Json::Value containing a serialized glm element
	 * @tparam T the glm type to deserialize the element
	 *
	 * @return a glm element containing deserialized data
	 */
	template <typename T>
	static T deGlm(Json::Value &glm) {
		using TypeT = typename T::value_type;
		const std::size_t n = sizeof(T) / sizeof(TypeT);

		if (!glm.isArray() || glm.size() != n) {
			std::cerr << "JSON array size does not match GLM type\n";
		}

		TypeT data[n];
		for (std::size_t i = 0; i < n; ++i)
			data[i] = glm[static_cast<Json::ArrayIndex>(i)].asFloat();

		T obj;
		std::memcpy(glm::value_ptr(obj), data, sizeof(TypeT) * n);
		return obj;
	}

	/**
	 * @brief Deserializes a glm quaternion element.
	 *
	 * @param elem the Json::Value containing a serialized glm quaternion element
	 *
	 * @warning It can be merged with deGlm
	 *
	 * @return a glm::quat containing the deserialized element
	 */
	inline glm::quat deQuat(Json::Value &elem) {
		auto v = deGlm<glm::vec4>(elem);
		return glm::quat(v.w, v.x, v.y, v.z);
	}

protected:
	/// used to store the content
	std::string m_name = "";
};

/**
 * @brief This class writes/read into/from a given file the json elements.
 */
class JsonSerializer : public Serializer<Json::Value> {
public:
	virtual ~JsonSerializer() = default;

	/**
	 * @brief Deserializes all the file content into a Json::Value.
	 *
	 * @param path the file path to be read
	 *
	 * @return a Json::Value containing all parsed elements
	 */
	virtual Json::Value deserializeFromFile(const std::string &path) override;

	/**
	 * @brief Serializes the Json::Value element given into a file.
	 *
	 * @param elem the Json::Value element to parse
	 * @param path the file path to create/write
	 */
	virtual void serializeToFile(Json::Value &elem, const std::string &path) override;

	JsonSerializer(JsonSerializer &other) = delete;

	void operator=(const JsonSerializer &other) = delete;

	/**
	 * @brief Retrieves the instance of the JsonSerializer.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<JsonSerializer> object
	 */
	inline static Shared<JsonSerializer> instance() {
		if (s_pointer == nullptr) {
			Shared<JsonSerializer> copy(new JsonSerializer());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

private:
	JsonSerializer() = default;

	/// static shared pointer for Singleton
	inline static Shared<JsonSerializer> s_pointer = nullptr;
};

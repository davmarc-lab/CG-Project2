#include "Serialize.hpp"

#include <cstring>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <json/json.h>

#include <stdexcept>
#include <type_traits>
#include <vector>

class JsonSerializable : public Serializable<Json::Value> {
public:
	virtual ~JsonSerializable() = default;

	virtual Json::Value serialize() { return Json::nullValue; }

	template <typename T>
	struct is_glm_type : std::false_type {};

	template <glm::length_t L, typename T, glm::qualifier Q>
	struct is_glm_type<glm::vec<L, T, Q>> : std::true_type {};

	template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	struct is_glm_type<glm::mat<C, R, T, Q>> : std::true_type {};

	template <typename T, glm::qualifier Q>
	struct is_glm_type<glm::qua<T, Q>> : std::true_type {};

	template <typename T>
	inline static Json::Value seGlmVec(std::vector<T> vec) {
		Json::Value arr(Json::arrayValue);
		if (is_glm_type<T>::value)
			for (auto e : vec) {
				arr.append(seGlm(e));
			}
		return arr;
	}

	template <typename T>
	inline static Json::Value seVec(std::vector<T> vec) {
		Json::Value arr(Json::arrayValue);
		for (auto e : vec)
			arr.append(e);
		return arr;
	}

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

	inline Json::Value seQuat(glm::quat &q) {
		auto vec = glm::vec4(q.x, q.y, q.z, q.w);
		return seGlm<glm::vec4>(vec);
	}

	virtual void deserialize(Json::Value &elem) {}

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

	template <typename T>
	inline static std::vector<T> deGlmVec(Json::Value &arr) {
		std::vector<T> data{};
		if (arr.isArray())
			for (auto e : arr) {
				data.push_back(deGlm<T>(e));
			}
		return data;
	}

	template <typename T>
	static T deGlm(Json::Value &arr) {
		using TypeT = typename T::value_type;
		const std::size_t n = sizeof(T) / sizeof(TypeT);

		if (!arr.isArray() || arr.size() != n) {
			std::cerr << "JSON array size does not match GLM type\n";
		}

		TypeT data[n];
		for (std::size_t i = 0; i < n; ++i)
			data[i] = arr[static_cast<Json::ArrayIndex>(i)].asFloat();

		T obj;
		std::memcpy(glm::value_ptr(obj), data, sizeof(TypeT) * n);
		return obj;
	}

	inline glm::quat deQuat(Json::Value &elem) {
		auto v = deGlm<glm::vec4>(elem);
		return glm::quat(v.w, v.x, v.y, v.z);
	}
};

class JsonSerializer : public Serializer<Json::Value> {
public:
	virtual ~JsonSerializer() = default;

	virtual void serializeToFile(Json::Value &elem, const std::string &path) override;
};

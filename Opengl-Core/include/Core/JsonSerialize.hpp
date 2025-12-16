#include "Serialize.hpp"

#include <cstring>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	inline static Json::Value seVec(T cont) {
		// glm packed value type
		using TypeT = typename T::value_type;
		// number of packed elements
		const size_t n = sizeof(T);

		TypeT data[n];
		std::memcpy(data, &cont[0], n);

		Json::Value arr(Json::arrayValue);
		for (size_t i = 0; i < n / sizeof(TypeT); i++)
			arr.append(data[i]);
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
	inline static T deVal(const T &val) {
		if (val)
			return val;
		return T();
	}

	// template <typename T>
	// static std::vector<T> deVec(Json::Value &arr) {
	// 	if (!arr.isArray() || arr.size() != n) {
	// 		std::cerr << "JSON array size does not match GLM type\n";
	// 	}
	//
	// 	std::vector<T> data{};
	// 	for (std::size_t i = 0; i < n; ++i)
	// 		data[i] = arr[static_cast<Json::ArrayIndex>(i)].asFloat();
	//
	// 	T obj;
	// 	std::copy(std::begin(data), std::end(data), std::begin(obj));
	// 	return obj;
	// }

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

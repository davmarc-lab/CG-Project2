#pragma once

#include <stdexcept>
#include <string>

template <class T>
class Serializable {
public:
	virtual ~Serializable() = default;

	virtual T serialize(T &elem) { throw std::runtime_error("Non callable method."); }

	virtual void deserialize(T &elem) {}
};

template <class T>
class Serializer {
public:
	virtual ~Serializer() = default;

	virtual T deserializeFromFile(const std::string &filePath) { return T(); }

	virtual void serializeToFile(T &elem, const std::string &filePath) {}
};

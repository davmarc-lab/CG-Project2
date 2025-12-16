#pragma once

#include <string>

template <class T>
class Serializable {
public:
	virtual ~Serializable() = default;

	virtual T serialize(T &elem) {}

	virtual void deserialize(T &elem) {}
};

template <class T>
class Serializer {
public:
	virtual ~Serializer() = default;

	virtual void serializeToFile(T &elem, const std::string& filePath) {}
};

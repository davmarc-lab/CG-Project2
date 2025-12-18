#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief This class provides overridable methods to serialize and deserialize a class.
 *
 * @tparam T the serialized data type
 */
template <class T>
class Serializable {
public:
	virtual ~Serializable() = default;

	/**
	 * @brief Defines how to serializes the data of the class that will extend Serializable.
	 *
	 * @tparam T the parameter and return data type
	 *
	 * @return the element serialized of type T
	 */
	virtual T serialize() { throw std::runtime_error("Non callable method."); }

	/**
	 * @brief Deserializes the content of the given element.
	 *
	 * @param elem the elem to deserialize
	 * @tparam T the parameter and return data type
	 */
	virtual void deserialize(T &elem) {}
};

/**
 * @brief This class writes/read into/from a given file the elements with the given type T.
 *
 * @tparam T the type of elements to serialize/deserialize.
 */
template <class T>
class Serializer {
public:
	virtual ~Serializer() = default;

	/**
	 * @brief Deserializes all the file content into a T element.
	 *
	 * @param filePath the file path to be read
	 * @tparam T the type of data to be parsed
	 *
	 * @return a T data containing all parsed elements
	 */
	virtual T deserializeFromFile(const std::string &filePath) { return T(); }

	/**
	 * @brief Serializes the element given of type T into a file.
	 *
	 * @param elem the element of type T to parse
	 * @param filePath the file path to create/write
	 * @tparam T the type of data to serialize
	 */
	virtual void serializeToFile(T &elem, const std::string &filePath) {}
};

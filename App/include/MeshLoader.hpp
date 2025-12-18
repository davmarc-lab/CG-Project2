#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <string>
#include <vector>

/**
 * @brief This class saves and loads entity from/to a file.
 * It serializes/deserializes all the entities components (which extends JsonSerializable).
 */
class MeshLoader {
public:
	/**
	 * @brief Loads a file where are contained all entitites serialized data.
	 * It creates the necessary entities and initializes all components needed.
	 *
	 * @param path the file path
	 *
	 * @return a vector containing all created entities
	 */
	std::vector<unsigned int> loadMeshes(const std::string &path);

	/**
	 * @brief Saves in a file placed in `resources/mesh/` called `scene.json` all the entities.
	 * In particular serializes the entities components data if they are allowed.
	 *
	 * @param entities the entities to serialize
	 *
	 * @return true if the file is written
	 */
	bool saveMeshes(std::vector<unsigned int> &entities);

	MeshLoader(MeshLoader &other) = delete;

	void operator=(const MeshLoader &other) = delete;

	/**
	 * @brief Retrieves the instance of the MeshLoader.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<MeshLoader> object
	 */
	inline static Shared<MeshLoader> instance() {
		if (s_pointer == nullptr) {
			Shared<MeshLoader> copy(new MeshLoader());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	~MeshLoader() = default;

private:
	MeshLoader() = default;

	/// static shared pointer for Singleton
	inline static Shared<MeshLoader> s_pointer = nullptr;
};

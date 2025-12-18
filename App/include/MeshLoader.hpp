#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <string>
#include <vector>

class MeshLoader {
public:
	std::vector<unsigned int> loadMeshes(const std::string &path);

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

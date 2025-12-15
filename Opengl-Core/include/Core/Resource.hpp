#pragma once

#include "Utils.hpp"

#include <map>
#include <string>

namespace ogl {

	/**
	 * @namespace res
	 * @brief Namespace used for Resource utilities
	 */
	namespace res {
		/// resource default folder
		const std::string DEFAULT_LOCATION = "./resources/";
	} // namespace res

	/**
	 * @brief This class provides method to manage a resource: a resource
	 * is a generic file stored in a precise location. By using Resource
	 * is simpler to manage the file path.
	 */
	class Resource {
	public:
		/**
		 * @brief Retrieves the resource location.
		 *
		 * @return the resource location
		 */
		inline std::string getResourceLocation() const { return this->m_location.c_str(); }

		/**
		 * @brief Retrieves the resource name.
		 *
		 * @return the resource name
		 */
		inline std::string getResourceName() const { return this->m_name.c_str(); }

		/**
		 * @brief Retrieves the resource file content.
		 *
		 * @return the resource content
		 */
		inline std::string getResourceContent() const { return this->m_content.c_str(); }

		/**
		 * @brief Retrieves the resource id.
		 *
		 * @return the resource id
		 */
		inline unsigned int getResourceId() const { return this->m_id; }

		/**
		 * @brief Sets the resource id.
		 *
		 * @param id the resource id
		 */
		inline void setResourceId(const unsigned int &id) { this->m_id = id; }

		/**
		 * @brief Retrieves the state of the resource.
		 *
		 * @return `true` if the resource is loaded in memory
		 */
		inline bool isResourceLoaded() const { return !this->m_content.empty(); }

		/**
		 * @brief Loads the resource in memory.
		 */
		void loadResource();

		/**
		 * @brief Remove the resource from memory.
		 */
		void unloadResource();

		Resource() = delete;

		/**
		 * @brief Instances a resource at the given file name and location.
		 *
		 * @param location resource folder
		 * @param file file name inside location
		 */
		Resource(const std::string &location, const std::string &file) :
			m_location(std::move(location)), m_file(std::move(file)) {
			this->m_name = std::string(this->m_file);
		}

		/**
		 * @brief Instances a resource with the given file name inside the default location.
		 *
		 * @param file the file name
		 *
		 * @see ogl::res::DEFAULT_LOCATION
		 */
		Resource(const std::string &file) :
			Resource(res::DEFAULT_LOCATION, file) {}

		~Resource() = default;

	private:
		/// resource id
		unsigned int m_id = 0;
		/// resource folder
		std::string m_location;
		/// resource file with extension
		std::string m_file;
		/// resource file name without extension
		std::string m_name;
		/// resource file content
		std::string m_content;
	};

	namespace res {
		/// empty resource used for validating resource loading
		const Resource EMPTY_RESOURCE = Resource("", "");
	} // namespace res

	/**
	 * @brief This singleton class manages and stores all resources for easy access.
	 *
	 * @note Not tested well.
	 */
	class ResourceManager {
	public:
		/**
		 * @brief Adds a resource to the manager.
		 *
		 * @param location the resource folder
		 * @param file the resource file
		 */
		unsigned int addResource(const std::string &location, const std::string &file);

		/**
		 * @brief Adds a resource located in the default location to the manager.
		 *
		 * @param file the resource file
		 */
		unsigned int addResource(const std::string &file);

		/**
		 * @brief Removes and unloads the resource with the given id.
		 *
		 * @param id resource id
		 *
		 * @return true if the resource is unloaded
		 */
		bool removeResource(const unsigned int &id);

		/**
		 * @brief Retrieves the current id used to store resources.
		 *
		 * @return the next id for a resource
		 */
		inline unsigned int getCurrentId() const { return this->m_currentId; }

		/**
		 * @brief Retrieve the resource with the given id.
		 *
		 * @param id resource id
		 *
		 * @return the Resource found
		 */
		Resource getResource(const unsigned int &id) const;

		/**
		 * @brief Clears the manager from all resources.
		 */
		void clear();

		ResourceManager(ResourceManager &other) = delete;

		void operator=(const ResourceManager &other) = delete;

		/**
		 * @brief Retrieves the instance of the ResourceManger.
		 * If it's not instanced, it will be instanced automatically.
		 *
		 * @return a Shared<ResourceManager> object
		 */
		inline static Shared<ResourceManager> instance() {
			if (s_pointer == nullptr) {
				Shared<ResourceManager> copy(new ResourceManager());
				copy.swap(s_pointer);
			}
			return s_pointer;
		}

	private:
		/// static shared pointer for Singleton
		inline static Shared<ResourceManager> s_pointer = nullptr;
		/// id tracker for resources
		unsigned int m_currentId = 1;

		/// map containing all indexed resources
		std::map<unsigned int, Resource> m_map{};

		ResourceManager() = default;
	};

} // namespace ogl

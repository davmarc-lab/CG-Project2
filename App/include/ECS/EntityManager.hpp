#pragma once

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief This class manages all the entities and the components instanced.
 *
 * It allows to add entities, add and remove components from entitites, query
 * all entities with a specific component, etc.
 *
 * It's implemented with singleton so there only be one instance of this class.
 */
class EntityManager {
public:
	using Index = unsigned int;

	/**
	 * @brief Create an entity and increment the current index.
	 *
	 * @return the id of the new entity
	 */
	inline Index createEntity() {
		auto id = this->m_currentId;
		this->m_currentId++;

		this->m_entities.emplace(id, "Entity " + std::to_string(id));
		this->m_ettComponent.emplace(id, std::vector<Shared<Component>>{});

		return id;
	}

	/**
	 * @brief Reset the ECS id, it is usually used when entities are cleaned.
	 */
	inline void resetIndex() {
		this->m_currentId = 0;
	}

	/**
	 * @brief Retrieves a vector containing all entities in the ECS.
	 *
	 * @return a vector of entities id.
	 */
	inline std::vector<Index> getEntities() const {
		std::vector<Index> res{};
		for (auto [id, _] : this->m_ettComponent) {
			res.push_back(id);
		}
		return res;
	}

	/**
	 * @brief Retrieves the entity name.
	 *
	 * @param id entity id
	 * @return the entity name
	 */
	inline std::string getEntityName(const Index &id) const { return this->m_entities.at(id); }

	/**
	 * @brief Sets the name of the entity given.
	 *
	 * @param id entity id
	 */
	inline void setEntityName(const Index &id, const std::string &name) { this->m_entities.at(id) = std::move(name); }

	/**
	 * @brief Removes the entity given from the ECS.
	 *
	 * If the entity is present in the ECS, his component will be cleared.
	 *
	 * @param id entity id
	 * @return `true` if the entity is removed
	 */
	inline bool removeEntity(const Index &id) {
		bool removed = this->m_entities.erase(id);
		for (auto c : this->m_ettComponent.at(id)) {
			c.reset();
		}
		this->m_ettComponent.erase(id);
		for (auto it = this->m_compEntity.begin(); it != this->m_compEntity.end();) {
			auto &ids = it->second;
			ids.erase(std::remove(ALL(ids), id), ids.end());

			if (ids.empty())
				it = this->m_compEntity.erase(it);
			else
				++it;
		}

		return removed;
	}

	/**
	 * @brief Checks if the entity given has components.
	 *
	 * @param id entity id
	 * @return `true` if the entity has components
	 */
	inline bool isEntityValid(const Index &id) { return this->m_ettComponent.count(id); }

	/**
	 * @brief Retrieves the number of entities in the ECS.
	 *
	 * @return the number of entities
	 */
	unsigned int getEntitiesCount() const { return this->m_entities.size(); }

	/**
	 * @brief Adds a component to the given entity.
	 * It instances a shared pointer of that component and calls the component
	 * constructor with the arguments given.
	 *
	 * @tparam T class of the component
	 * @tparam Args list of arguments
	 * @param id entity id
	 * @param args arguments list
	 * @return a shared pointer to that component of type T
	 */
	template <typename T, typename... Args>
	inline Shared<T> addComponent(const Index &id, Args &&...args) {
		if (!this->isEntityValid(id))
			return nullptr;

		auto comp = CreateShared<T>(std::forward<Args>(args)...);
		this->m_ettComponent.at(id).emplace_back(comp);

		auto name = CLASSNAME(T);
		if (this->m_compEntity.count(name)) {
			this->m_compEntity.at(name).push_back(id);
		} else {
			this->m_compEntity.emplace(name, std::vector<Index>{});
			this->m_compEntity.at(name).push_back(id);
		}
		return comp;
	}

	/**
	 * @brief Removes a component from the given entity.
	 *
	 * @tparam T class of the component that will be removed
	 *
	 * @param id entity id
	 * @return true if the component is removed
	 */
	template <typename T>
	inline bool removeComponent(const Index &id) {
		if (!this->entityHasComponent<T>(id))
			return false;

		this->m_compEntity.at(CLASSNAME(T)).erase(std::find(ALL(this->m_compEntity.at(CLASSNAME(T))), id));
		auto c = this->getComponentFromId<T>(id);
		if (c != nullptr)
			this->m_ettComponent.at(id).erase(std::find(ALL(this->m_ettComponent.at(id)), c));
		return this->entityHasComponent<T>(id);
	}

	/**
	 * @brief Checks if the given entity has the component.
	 *
	 * @tparam T class of the component
	 * @param id entity id
	 * @return true if the entity has that component
	 */
	template <typename T>
	inline bool entityHasComponent(const Index &id) {
		return this->m_compEntity.count(CLASSNAME(T)) > 0 &&
			std::find(ALL(this->m_compEntity.at(CLASSNAME(T))), id) != this->m_compEntity.at(CLASSNAME(T)).end();
	}

	/**
	 * @brief Retrieves all the entities that have the given component.
	 *
	 * @tparam T class of the component
	 *
	 * @return a vector containing entities id.
	 */
	template <typename T>
	inline std::vector<Index> getEntitiesFromComponent() const {
		if (!this->m_compEntity.count(CLASSNAME(T)))
			return {};

		return this->m_compEntity.at(CLASSNAME(T));
	}

	/**
	 * @brief Retrieves a shared pointer to the component of an entity.
	 *
	 * @tparam T class of the component
	 * @param id entity id
	 * @return a shared pointer to the component if the entity has one, if not
	 * it returns `nullptr`
	 */
	template <typename T>
	inline Shared<T> getComponentFromId(const Index &id) {
		if (!this->m_compEntity.count(CLASSNAME(T)))
			return nullptr;

		// the entity with id hasn't this component.
		if (std::find(ALL(this->m_compEntity.at(CLASSNAME(T))), id) == this->m_compEntity.at(CLASSNAME(T)).end()) {
			// std::cerr << "The entity with id (" << id << ") doesn't have this component (" << CLASSNAME(T) << ").\n";
			return nullptr;
		}

		// finds T component by using dynamic cast.
		return std::dynamic_pointer_cast<T>(*std::find_if(ALL(this->m_ettComponent.at(id)), [](auto e) {
			return std::dynamic_pointer_cast<T>(e) != nullptr;
		}));
	}

	EntityManager(EntityManager &other) = delete;

	void operator=(const EntityManager &other) = delete;

	/**
	 * @brief Retrieves the instance of the EntityManager.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<EntityManager> object
	 */
	inline static Shared<EntityManager> instance() {
		if (s_pointer == nullptr) {
			Shared<EntityManager> copy(new EntityManager());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

private:
	/// current ecs index
	Index m_currentId = 0;

	/// map containing each entity and its name
	std::map<Index, std::string> m_entities{};
	/// map containing each entity and its components
	std::map<Index, std::vector<Shared<Component>>> m_ettComponent{};
	/// map containing each component class and all the entities with that component
	std::map<std::string, std::vector<Index>> m_compEntity{};

	/// static shared pointer for Singleton
	inline static Shared<EntityManager> s_pointer = nullptr;

	/// @brief Default constructor
	EntityManager() = default;
};

#include "../../../Opengl-Core/include/Core.hpp"

/// event dispatched when player dies
const auto PLAYER_DEAD_EVENT = ogl::Event("Player Dead");

#pragma once

#include <map>
#include "../../../Opengl-Core/include/Core.hpp"

/**
 * @brief This class allow to create a state to be attached to the StateManager.
 * A State works like a scene: defines all the meshes that will be rendered or not,
 * defines custom behaviour while updating mesh data or while rendering meshes.
 *
 * This is an abstract class it needs to be extended and override the methods that
 * the child State will use.
 *
 * It can be used to create scenes with meshes, lights, skybox etc.
 * A State can have custom behaviour while updating and rendering, every "static"
 * mesh must be initialized in State::onAttach() implementation.
 *
 * @note This application when executed will load a state loader (scene picker),
 * where the user choose which state (scene) will be loaded.
 */
class State {
public:
	/**
	 * @brief This method is called after using `StateManager::sync()` (read `StateMnaager::sync()` documentation for more info).
	 * It should contain all the operations that must be done before you start the execution of
	 * a `State`.
	 *
	 * Also this method need to be called in child class implementation using `State::onAttach()`
	 */
	virtual void onAttach() {
		std::cout << "Attaching state: " << this->m_name << " to StateManager\n";
		this->m_attached = true;
	}

	/**
	 * @brief This method is called while using `sync()` (read `sync()` documentation for more info).
	 * It should contain all the operations that must be done at the end of `State` execution
	 * before deleting or changing `State`, it is automatically called by `sync()`.
	 *
	 * Also this method need to be called in child class implementation using `State::onDetach()`
	 */
	virtual void onDetach() {
		std::cout << "Detaching state: " << this->m_name << " from StateManager\n";
		this->m_attached = false;
	}

	/**
	 * @brief This methods contains all the operations executed at every `event::loop::LOOP_UPDATE`
	 * if there are any.
	 */
	virtual void onUpdate() {}

	/**
	 * @brief This methods contains all the operations executed at every `event::loop::LOOP_RENDER`
	 * if there are any.
	 */
	virtual void onRender() {}

	/**
	 * @brief Retrieves the current binary state of the current `State`
	 *
	 * @return `bool` true if the state is finished
	 */
	virtual bool isCurrentStateEnd() { return true; }

	/** @brief Retrieves the `State` name.
	 *
	 * @return a strng with the state's name
	 */
	std::string getName() const { return this->m_name; }

	/** @brief Retrieves if the state is attached.
	 *
	 * @return `true` if the state is attached
	 */
	bool isAttached() const { return this->m_attached; }

	/**
	 * @brief Instances a state with the given name.
	 *
	 * @note Each state need a name to be cached in the StateManager.
	 *
	 * @params name the state's name
	 */
	State(const std::string &name = "State") :
		m_name(std::move(name)) {}
	virtual ~State() = default;

protected:
	/// state's name
	const std::string m_name;
	/// true if the state is attached
	bool m_attached = false;
};

/**
 * @brief This class manages the current State running in the application and
 * also provides a simple cache system.
 *
 * It allow to change state while executing for one iteration or in a loop using
 * a simple barrier system.
 *
 * It's implemented with singleton so there only be one instance of this class.
 */
class StateManager {
public:
	StateManager(StateManager &other) = delete;

	void operator=(const StateManager &other) = delete;

	/**
	 * @brief Retrieves the instance of the StateManager if it's not created.
	 * This function is thread safe using a simple `std::mutex`.
	 *
	 * @return `StateManager` unique object.
	 */
	inline static Shared<StateManager> instance() {
		if (s_pointer == nullptr) {
			Shared<StateManager> copy(new StateManager());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	/**
	 * @brief Retrieves `true` if the current state should exit.
	 *
	 * @return true if the current state is finished or changed
	 */
	bool shouldExit() const {
		ASSERT(this->m_currentState != nullptr);
		return this->m_currentState->isCurrentStateEnd();
	}

	/**
	 * @brief Execute `State::onUpdate()` method of the current state.
	 */
	void execUpdate() { this->m_currentState->onUpdate(); }

	/**
	 * @brief Execute `State::onRender()` method of the current state.
	 */
	void execRender() { this->m_currentState->onRender(); }

	/**
	 * @brief This method works like a barrier: after calling `StateManager::changeState()`,
	 * detach the current state; clear the `EventManager`, attach the queued `State`,
	 * defines new event listeners.
	 * It needs to be called after every `StateManager::changeState()` or at the end
	 * of a loop before calling `StateManager::shouldExit()`.
	 *
	 * @note It uses a `bool` variable to check if there is a new `State` in the queue.
	 */
	void sync();

	/**
	 * @brief Cleans all the current state cache system.
	 */
	void clean();

	/**
	 * @brief Works as the other `StateManager::changeState(const Shared<State>&)` but this
	 * will use the given `name` while caching.
	 *
	 * @see StateManager::changeState(const Shared<State>&)
	 *
	 * @param name state's name
	 * @param State a shared pointer to the `State`
	 */
	void changeState(const std::string &name, const Shared<State> &state);

	/**
	 * @brief This method queue the given `State` and tells the `StateManager` that the current
	 * state needs to be changed.
	 * If the given `State` is not in the cache system, the `StateManager` will cache the `State`
	 * with the state's name.
	 * To make it work the method `StateManager::sync()` needs to be called
	 * (read `StateManager::sync()` for further informations).
	 *
	 * @param State a shared pointer to the `State`
	 */
	inline void changeState(const Shared<State> &state) { this->changeState(state->getName(), state); }
	/**
	 * @brief This method search in the cache the `State` with the given name.
	 * If the name is not found in the cache, the current state will not change.
	 *
	 * @param name the state's name
	 */
	void changeState(const std::string &name);

	/**
	 * @brief Retrieves if the current state is defined or not.
	 *
	 * @return true if the current state is initialized
	 */
	bool isCurrentStateDefined() const { return this->m_currentState != nullptr; }

	/**
	 * @brief This method caches the `State` given with the given name.
	 *
	 * @param name state's name
	 * @param State a shared pointer to the `State`
	 */
	void cacheState(const std::string &name, const Shared<State> &state);

	/**
	 * @brief Caches the given State, his name will be used in the cached states.
	 *
	 * @param state state to be cached
	 */
	inline void cacheState(const Shared<State> &state) { this->cacheState(state->getName(), state); }

	/**
	 * @brief Removes the `State` with the given name if contained in the cache.
	 *
	 * @param name state's name
	 */
	void uncacheState(const std::string &name);

	/**
	 * @brief Retrieves the current cache of the `StateManager`.
	 *
	 * @return cachedStates map of (`std::string`, `Shared<State>`)
	 */
	inline std::map<std::string, Shared<State>> getCachedStates() { return this->m_cacheStates; }

	~StateManager() = default;

private:
	/// static shared pointer for Singleton
	inline static Shared<StateManager> s_pointer = nullptr;

	/// pointer to the current state
	Shared<State> m_currentState = nullptr;
	/// pointer to the queued state
	Shared<State> m_queueState = nullptr;
	/// map of cached states
	std::map<std::string, Shared<State>> m_cacheStates{};

	/// true if the state is changed and queued state is not nullptr
	bool m_stateChanged = false;

	StateManager() = default;
};

#pragma once

#include <map>
#include "../../../Opengl-Core/include/Core.hpp"

class State {
public:
	/**
	 * This method is called after using `StateManager::sync()` (read `StateMnaager::sync()` documentation for more info).
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
	 * This method is called while using `sync()` (read `sync()` documentation for more info).
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
	 * This methods contains all the operations executed at every `event::loop::LOOP_UPDATE`
	 * if there are any.
	 */
	virtual void onUpdate() {}

	/**
	 * This methods contains all the operations executed at every `event::loop::LOOP_RENDER`
	 * if there are any.
	 */
	virtual void onRender() {}

	/**
	 * Retrieves the current binary state of the current `State`
	 *
	 * @return `bool` true if the state is finished
	 */
	virtual bool isCurrentStateEnd() { return true; }

	/* Retrieves the `State` name.*
	 *
	 * @return `std::string` state's name
	 */
	std::string getName() const { return this->m_name; }

	/* Retrieves if the state is attached.*
	 *
	 * @return `true` if the state is attached
	 */
	bool isAttached() const { return this->m_attached; }

	/**
	 * It needs a name for each State in case you want to use State caching.
	 *
	 * @params name the state's name
	 */
	State(const std::string &name = "State") :
		m_name(std::move(name)) {}
	virtual ~State() = default;

protected:
	const std::string m_name;
	bool m_attached = false;
};

class StateManager {
public:
	StateManager(StateManager &other) = delete;

	void operator=(const StateManager &other) = delete;

	/**
	 * Retrieves the instance of the StateManager if it's not created.
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
	 * Retrieves `true` if the current state should exit.
	 * If the current state is detached, this will throw an error.
	 *
	 * @reutrn `bool`
	 */
	bool shouldExit() const {
		ASSERT(this->m_currentState != nullptr);
		return this->m_currentState->isCurrentStateEnd();
	}

	/**
	 * Execute `State::onUpdate()` method of the current state.
	 */
	void execUpdate() { this->m_currentState->onUpdate(); }
	/**
	 * Execute `State::onRender()` method of the current state.
	 */
	void execRender() { this->m_currentState->onRender(); }

	/**
	 * This method works like a barrier: after calling `StateManager::changeState()`,
	 * detach the current state; clear the `@see EventManager`, attach the queued `State`,
	 * defines new event listeners.
	 * It needs to be called after every `StateManager::changeState()` or at the end
	 * of a loop before calling `StateManager::shouldExit()`.
	 *
	 * @note It uses a `bool` variable to check if there is a new `State` in the queue.
	 */
	void sync();

	/**
	 * Cleans all the current state cache system.
	 */
	void clean();

	/**
	 * Works as the other `StateManager::changeState(const Shared<State>&)` but this
	 * will use the given `name` while caching.
	 *
	 * @see StateManager::changeState(const Shared<State>&)
	 *
	 * @param name state's name
	 * @param State a shared pointer to the `State`
	 */
	void changeState(const std::string &name, const Shared<State> &state);

	/**
	 * This method queue the given `State` and tells the `StateManager` that the current
	 * state needs to be changed.
	 * If the given `State` is not in the cache system, the `StateManager` will cache the `State`
	 * with the state's name.
	 * To make it work the method `StateManager::sync()` needs to be called
	 * (read `@see StateManager::sync()` for further informations).
	 *
	 * @param State a shared pointer to the `State`
	 */
	inline void changeState(const Shared<State> &state) { this->changeState(state->getName(), state); }
	/**
	 * This method search in the cache the `State` with the given name.
	 * If the name is not found in the cache, the current state will not change.
	 *
	 * @param name the state's name
	 */
	void changeState(const std::string &name);

	/**
	 * Retrieves if the current state is defined or not.
	 *
	 * @return true if the current state is initialized
	 */
	bool isCurrentStateDefined() const { return this->m_currentState != nullptr; }

	/**
	 * This method caches the `State` given with the given name.
	 *
	 * @param name state's name
	 * @param State a shared pointer to the `State`
	 */
	void cacheState(const std::string &name, const Shared<State> &state);
	inline void cacheState(const Shared<State> &state) { this->cacheState(state->getName(), state); }

	/**
	 * Removes the `State` with the given name if contained in the cache.
	 *
	 * @param name state's name
	 */
	void uncacheState(const std::string &name);

	/**
	 * Retrieves the current cache of the `StateManager`.
	 *
	 * @return cachedStates map of (`std::string`, `Shared<State>`)
	 */
	inline std::map<std::string, Shared<State>> getCachedStates() { return this->m_cacheStates; }

	~StateManager() = default;

private:
	inline static Shared<StateManager> s_pointer = nullptr;

	Shared<State> m_currentState = nullptr;
	Shared<State> m_queueState = nullptr;
	std::map<std::string, Shared<State>> m_cacheStates{};

	bool m_stateChanged = false;

	StateManager() = default;
};

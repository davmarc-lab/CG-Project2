#pragma once

#include <map>
#include "../../../Opengl-Core/include/Core.hpp"

class State {
public:
	/*
	 * This method is called after using `sync()` (read `sync()` documentation for more info).
	 * It should contain all the operations that must be done before you start the execution of
	 * a `State`.
	 *
	 * Also this method need to be called in child class implementation using `State::onAttach()`
	 */
	virtual void onAttach() {
		std::cout << "Attaching state: " << this->m_name << " to StateManager\n";
		this->m_attached = true;
	}

	/*
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

	/*
	 * This methods contains all the operations that will be executing at every `event::loop::LOOP_UPDATE`
	 * if there are any.
	 */
	virtual void onUpdate() {}

	/*
	 * This methods contains all the operations that will be executing at every `event::loop::LOOP_RENDER`
	 * if there are any.
	 */
	virtual void onRender() {}

	/*
	 * Retrieves the current binary state of the current `State`
	 *
	 * @return `bool` false if the state is finished
	 */
	virtual bool isCurrentStateEnd() { return true; }

	/// Retrieves the `State` name.
	///
	/// @return `std::strin` state's name
	std::string getName() const { return this->m_name; }

	/// Retrieves if the state is attached.
	///
	/// @return `true` if the state is attached
	bool isAttached() const { return this->m_attached; }

	/*
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

	inline static Shared<StateManager> instance() {
		if (s_pointer == nullptr) {
			Shared<StateManager> copy(new StateManager());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	bool shouldExit() const {
		ASSERT(this->m_currentState != nullptr);
		return this->m_currentState->isCurrentStateEnd();
	}

	void execUpdate() { this->m_currentState->onUpdate(); }
	void execRender() { this->m_currentState->onRender(); }

	void sync();

	void clean();

	void changeState(const std::string &name, const Shared<State> &state);
	void changeState(const std::string &name);

	bool isCurrentStateDefined() const { return this->m_currentState != nullptr; }

	void cacheState(const std::string &name, const Shared<State> &state);
	void uncacheState(const std::string &name);
	void uncacheState(const Shared<State> &state);

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

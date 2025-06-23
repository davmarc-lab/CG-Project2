#pragma once

#include <map>
#include "../../../Opengl-Core/include/Core.hpp"

class State {
public:
	/// This methods need to be called in child class implementation using `State::onAttach()`
	virtual void onAttach() { this->m_attached = true; }
	/// This methods need to be called in child class implementation using `State::onDetach()`
	virtual void onDetach() { this->m_attached = false; }

	virtual void onUpdate() {}
	virtual void onRender() {}

	std::string getName() const { return this->m_name; }
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

	void execUpdate() { this->m_currentState->onUpdate(); }
	void execRender() { this->m_currentState->onRender(); }

	void clean();

	void changeState(const std::string &name, const Shared<State> &state);

	bool isCurrentStateDefined() const { return this->m_currentState != nullptr; }

	void cacheState(const std::string &name, const Shared<State> &state);
	void uncacheState(const std::string &name);
	void uncacheState(const Shared<State> &state);

	~StateManager() = default;

private:
	inline static Shared<StateManager> s_pointer = nullptr;

	Shared<State> m_currentState = nullptr;
	std::map<std::string, Shared<State>> m_cacheStates{};

	StateManager() = default;
};

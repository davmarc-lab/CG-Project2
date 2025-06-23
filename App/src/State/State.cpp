#include "../../include/State/State.hpp"

void StateManager::clean() {
    // detach the current state before everything
	if (this->m_currentState != nullptr) {
		if (!this->m_currentState->isAttached())
			this->m_currentState->onDetach();
		// remove it from cached states
		this->m_cacheStates.erase(this->m_currentState->getName());
	}

	// detach all cached states
	for (auto [_, state] : this->m_cacheStates) {
		if (state->isAttached())
			state->onDetach();
	}
}

void StateManager::changeState(const std::string &name, const Shared<State> &state) {
	// if there is a state clean that
	if (this->m_currentState != nullptr)
		this->m_currentState->onDetach();

	if (!this->m_cacheStates.count(name)) {
		this->m_cacheStates.insert_or_assign(name, state);
	}
	ASSERT(this->m_cacheStates.count(name));
	ASSERT(this->m_cacheStates.at(name) != nullptr);

	this->m_currentState = this->m_cacheStates.at(name);
	this->m_currentState->onAttach();
}

void StateManager::cacheState(const std::string &name, const Shared<State> &state) {
	if (this->m_cacheStates.count(name)) {
		std::cerr << "Cache already contains a State with that name: " << name << "\n";
		return;
	}
	this->m_cacheStates.insert_or_assign(name, state);
}
void StateManager::uncacheState(const std::string &name) {
	if (this->m_cacheStates.count(name))
		this->m_cacheStates.erase(name);
}
void StateManager::uncacheState(const Shared<State> &state) {
	std::cerr << "USE THE OTHER ONE, MORE EFFICIENT\n";
	ASSERT(false);
}

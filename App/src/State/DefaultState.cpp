#include "../../include/State/DefaultState.hpp"

void DefaultState::onAttach() {
	std::cout << "INIT\n";
	State::onAttach();
}

void DefaultState::onDetach() {
	std::cout << "CLEAN\n";
	State::onDetach();
}

void DefaultState::onUpdate() {
	std::cout << "UPDATING\n";
}

void DefaultState::onRender() {
	std::cout << "RENDERING\n";
}

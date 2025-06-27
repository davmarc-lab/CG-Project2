#pragma once

#include "Utils.hpp"

#include <array>
#include <mutex>
#include <vector>

const int MAX_KEY_QUEUE = 20;
const int MAX_MOUSE_KEY_QUEUE = 8;

struct Key {
	unsigned int key;
	unsigned int status;
};

namespace ogl {
	class InputManager {
	public:
		InputManager(InputManager &other) = delete;

		void operator=(const InputManager &other) = delete;

		/*
		 * Retrieves the instance of the InputManager if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `InputManager` unique object.
		 */
		inline static Shared<InputManager> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<InputManager> copy(new InputManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		void keyPressed(const unsigned int &key);

		void keyReleased(const unsigned int &key);

		bool isKeyPressed(const unsigned int &key) const;

		Key getKeyStatus(const unsigned int &key) const;

		std::vector<Key> getKeys();

	private:
		inline static Shared<InputManager> s_pointer = nullptr;
		inline static std::mutex s_mutex{};

		std::vector<Key> m_keys = std::vector<Key>();
		std::array<Key, MAX_MOUSE_KEY_QUEUE> m_mouse{};

		InputManager() = default;
	};
} // namespace ogl

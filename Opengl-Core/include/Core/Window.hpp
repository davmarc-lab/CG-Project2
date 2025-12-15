#pragma once

#include "../Graphic.hpp"

#include "Layer.hpp"
#include "Structs.hpp"
#include "Utils.hpp"

#include <functional>

#include <string>

namespace ogl {

	/**
	 * @brief Data structure to store all Window callbacks.
	 */
	struct WindowCallbacks {
		/// resize callback
		std::function<void(GLFWwindow *, int, int)> resizeCallback;
		/// key callback
		std::function<void(GLFWwindow *, int, int, int, int)> keyCallback = nullptr;
		/// mouse button callback
		std::function<void(GLFWwindow *, int, int, int)> mouseButtonCallback = nullptr;
		/// cursor position callback
		std::function<void(GLFWwindow *, double, double)> cursorPosCallback = nullptr;
	};

	/**
	 * @brief Data structure to store all window settings.
	 */
	struct WindowSettings {
		/// window name
		std::string name = "GLFW Window";
		/// window size
		Pair<float> size{1600, 900};
		/// window position
		Pair<int> position{0, 0};

		/// is window maximized
		bool maximized = false;
		/// is window fullscreen
		bool fullscreen = false;
		/// is window focused
		bool focused = false;
		/// is window decorated
		bool decorated = true;
		/// is vsync enabled
		bool vsync = false;
		/// enable debug api calls
		bool debugMode = false;
		/// window background color
		Color bgColor = {0.3f, 0.3f, 0.3f, 1.0f};
	};

	/**
	 * @brief This class provides methods to create and manage GLFW windows.
	 * It inherit Layer class.
	 */
	class Window : public Layer {
	public:
		/// basic constructor
		Window(const WindowSettings &settings);
		Window() = delete;
		~Window() = default;

		/**
		 * @brief Retrieves the GFLWWindow context for the current window.
		 *
		 * @return the pointer to the context
		 */
		inline GLFWwindow *getContext() const { return this->m_context; }

		/**
		 * @brief Retrieves the window size.
		 *
		 * @return a Pair containing the window width and height
		 */
		inline Pair<float> getSize() const { return this->m_settings.size; }

		/**
		 * @brief Retrieves the window width.
		 *
		 * @return the window width
		 */
		inline float getWidth() const { return this->m_settings.size.x; }

		/**
		 * @brief Sets the window width.
		 *
		 * @param val the new width
		 */
		void setWidth(const float &val) { this->m_settings.size.x = val; }

		/**
		 * @brief Retrieves the window height.
		 *
		 * @return the window height
		 */
		inline float getHeight() const { return this->m_settings.size.y; }

		/**
		 * @brief Sets the window height.
		 *
		 * @param val the new height
		 */
		void setHeight(const float &val) { this->m_settings.size.y = val; }

		/**
		 * @brief Toggle the vsync of the window.
		 */
		void toggleVsync();

		/**
		 * @brief Returns true if the vsync is enabled.
		 *
		 * @return true if vsync is enabled
		 */
		bool isVsyncEnabled() const;

		/**
		 * @brief Add the given value to the clear mask.
		 *
		 * @param val the value to add
		 */
		void addClearMask(const unsigned int &val) { this->m_clearMask |= val; }

		/**
		 * @brief Sets the resize callback of the window.
		 *
		 * @param func the resize callback
		 */
		void setResizeCallback(std::function<void(GLFWwindow *, int, int)> &&func);

		/**
		 * @brief Executes the resize callback.
		 *
		 * @param context the glfw context
		 * @param width the new width
		 * @param height the new height
		 */
		void execResizeCallback(GLFWwindow *context, const int &width, const int &height);

		/**
		 * @brief Sets the keyboard callback of the window.
		 *
		 * @param func the keyboard callback
		 */
		void setKeysCallback(std::function<void(GLFWwindow *, int, int, int, int)> &&func);

		/**
		 * @brief Executes the keyboard callback.
		 *
		 * @param context the glfw context
		 * @param key the key value
		 * @param code the key code
		 * @param action the key action
		 * @param mods the key modifiers
		 */
		void execKeysCallback(GLFWwindow *context, const int &key, const int &code, const int &action, const int &mods);

		/**
		 * @brief Sets the mouse button callback of the window.
		 *
		 * @param func the mouse button callback
		 */
		void setMouseButtonCallback(std::function<void(GLFWwindow *, int, int, int)> &&func);

		/**
		 * @brief Executes the mouse button callback.
		 *
		 * @param context the glfw context
		 * @param button the button value
		 * @param action the button action
		 * @param mods the button modifiers
		 */
		void execMouseButtonCallback(GLFWwindow *context, const int &button, const int &action, const int &mods);

		/**
		 * @brief Sets the cursor position callback of the window.
		 *
		 * @param func the cursor position callback
		 */
		void setCursorPosCallback(std::function<void(GLFWwindow *, double, double)> &&func);

		/**
		 * @brief Executes the cursor position callback of the window.
		 *
		 * @param context the glfw context
		 * @param xpos the mouse x position on the screen
		 * @param ypos the mouse y position on the screen
		 */
		void execCursorPosCallback(GLFWwindow *context, const double &xpos, const double &ypos);

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate() override;
		virtual void onRender() override;

		virtual void begin() override;
		virtual void end() override;

	private:
		/// the glfw context
		GLFWwindow *m_context = nullptr;
		/// the window settings
		WindowSettings m_settings{};
		/// the window clear mask
		unsigned int m_clearMask = 0;

		/// the window callbacks
		WindowCallbacks m_callbacks{};

		/// updates the user pointer inside glfw context
		void updateUserPointer();
	};

} // namespace ogl

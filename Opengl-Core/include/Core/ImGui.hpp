#pragma once

#include "Layer.hpp"
#include "Utils.hpp"
#include "Window.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ogl {
	const ImGuiConfigFlags DEFAULT_IMGUI_CONFIGS{ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable};

	/**
	 * @brief ImGui wrap to create a simple ImGui panel.
	 * Objects extending this class should be used in the ImGuiManger
	 *
	 * @see ImGuiManager
	 */
	class ImGuiPanel {
	public:
		/**
		 * @brief Retrieves layer priority.
		 *
		 * The priority is used to organize layer callback methods.
		 *
		 * @return the layer priority
		 */
		inline unsigned short getPriority() const { return this->m_priority; }

		/**
		 * @brief Updates panel priority with the given value.
		 *
		 * @param priority panel new priority
		 */
		inline void setPriority(const unsigned short &priority) {
			// EVENT: ig_layer_priority_changed
			this->m_priority = priority;
		}

        /**
         * @brief Initializes the current panel.
         *
         */
		inline virtual void onAttach() {
			if (this->m_attached)
				return;
			this->m_attached = true;
		}

		/**
		 * @brief Sets the render function of the panel.
		 * The render function contains all operations for drawing ImGui elements.
		 *
		 * @param func panel render function
		 */
		inline void setRenderFunc(std::function<void()> &&func) { this->m_renderFunc = std::move(func); }

		/**
		 * @brief Method called at every frame that draws the ImGui window.
		 *
		 * When extending this class this method can be ovverrided. When an
		 * object of this class is instanced, the method setRenderFunc() must be called.
		 */
		inline virtual void onRender() { this->m_renderFunc(); }

		/**
		 * @brief Instances basic panel with the given name.
		 *
		 * @param name panel name
		 * @param priority panel priority
		 */
		ImGuiPanel(const std::string &name = "Panel", const unsigned short &priority = 0) :
			m_name(std::move(name)), m_priority(priority) {}

		virtual ~ImGuiPanel() = default;

	private:
		/// panel name
		std::string m_name = "Panel";
		/// panel priority
		unsigned short m_priority = 0;
		/// panle attached flag
		bool m_attached = false;

		/// panel render func
		std::function<void()> m_renderFunc = []() {};
	};

	/**
	 * @brief This class manages all ImGuiPanel used in a State.
	 */
	class ImGuiManager : public Layer {
	public:
		ImGuiManager() = delete;

		/**
		 * @brief Instances an ImGuiManager for the given window using the given ImGui flags
		 * for all panels.
		 *
		 * @param layerName layer name
		 * @param window target window
		 * @param flags ImGui config flags
		 */
		ImGuiManager(const std::string &layerName, Window *window, const ImGuiConfigFlags &flags = 0) :
			Layer(std::move(layerName)), m_window(window), m_flags(flags) {}

		/**
		 * @brief Instances an ImGuiManager for the given window using the given ImGui flags
		 * for all panels.
		 *
		 * @param window target window
		 * @param flags ImGui config flags
		 */
		ImGuiManager(Window *window, const ImGuiConfigFlags &flags) :
			ImGuiManager("ImGui Manager", window, flags) {}

		/**
		 * @brief Adds an ImGuiPanel to the manager.
		 * This method creates the panel and calls its constructor using the given params.
		 *
		 * @tparam T ImGuiPanel inherited class
		 * @tparam Args constructor forwarded parameters
		 *
		 * @param args all constructor params
		 *
		 * @return a Shared<T> object
		 */
		template <typename T, typename... Args>
		inline Shared<T> addPanel(Args &&...args) {
			if (!this->m_attached)
				return nullptr;

			Shared<T> elem = CreateShared<T>(std::forward<Args>(args)...);
			elem->onAttach();

			if (this->m_panels.find(elem->getPriority()) == this->m_panels.end()) {
				this->m_panels.insert_or_assign(elem->getPriority(), std::vector<Shared<ImGuiPanel>>());
			}

			this->m_panels.at(elem->getPriority()).push_back(elem);
			return elem;
		}

		/**
		 * @brief Removes the given panel from the manager.
		 *
		 * @tparam inherited class of panel pointer
		 *
		 * @param panel panel pointer to removed
		 *
		 * @return true if the panel is removed
		 */
		template <typename T>
		inline bool removePanel(const Shared<ImGuiPanel> &panel) {
			auto prio = panel->getPriority();
			auto elem = std::find(ALL(this->m_panels.at(prio)), panel);
			if (elem != this->m_panels.at(prio).end()) {
				this->m_panels.at(prio).erase(elem);
				return true;
			}
			return false;
		}

		/**
		 * @brief Adds the given panel to the manager
		 *
		 * @param panel the panel to add
		 */
		inline void addPanel(const Shared<ImGuiPanel> &panel) {
			if (this->m_panels.find(panel->getPriority()) == this->m_panels.end()) {
				this->m_panels.insert_or_assign(panel->getPriority(), std::vector<Shared<ImGuiPanel>>());
			}

			this->m_panels.at(panel->getPriority()).push_back(panel);
		}

		/**
		 * @brief Retrieves all the panels of the manager with the given priority.
		 *
		 * @return vector containing all panels with the given priority
		 */
		std::vector<Shared<ImGuiPanel>> getPanels(const unsigned short &priority) { return {this->m_panels.at(priority)}; }

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate() override;
		virtual void onRender() override;

		virtual void begin() override;
		virtual void end() override;

	private:
		/// imgui config flags of the manager
		ImGuiConfigFlags m_flags = DEFAULT_IMGUI_CONFIGS;
		/// pointer to the window
		Window *m_window;

		/// map containing all panels for each priority
		std::map<unsigned short, std::vector<Shared<ImGuiPanel>>> m_panels{};
	};

} // namespace ogl

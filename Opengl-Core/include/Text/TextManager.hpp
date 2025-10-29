#pragma once

#include "../../include/Buffer/VertexArray.hpp"
#include "../../include/Buffer/VertexBuffer.hpp"
#include "../../include/Core/Layer.hpp"
#include "../../include/Core/Utils.hpp"
#include "../../include/Shader/Shader.hpp"

#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace ogl {
	/**
	 * @brief Data structure for Text settings in the manager.
	 */
	struct TextSettings {
		/// text font family file
		std::string fontFace = "./resources/fonts/arial.ttf";
		/// text pixel width
		unsigned int pixelWidth = 0;
		/// text pixel height
		unsigned int pixelHeight = 48;
		/// max char in font family
		unsigned int numChars = 128;
	};

	/**
	 * @brief Helper data structure to create Text in the 2D scene.
	 */
	struct TextHelper {
		/// text content
		std::string text = "Simple Text";
		/// text position
		glm::vec2 position = {0, 0};
		/// text scale
		float scale = 1.f;
		/// text color
		glm::vec3 color = {1, 0, 0};
	};

	/**
	 * @brief This class creates a text object to add in a 2D scene.
	 */
	class Text {
	public:
		/**
		 * @brief Initialize the text.
		 *
		 * @warning The OpenGL context should be already initialized (glad or glew).
		 */
		void init();

		/**
		 * @brief Binds the text VertexArray.
		 */
		inline void bindVAO() { this->m_vao.bind(); }

		/**
		 * @brief Binds the text VertexBuffer.
		 */
		inline void bindVBO() { this->m_vbo.bind(); }

		// inline unsigned int getVboId() { return this->m_vbo.getId(); }

		/**
		 * @brief Changes the text content.
		 *
		 * @param text the new text content
		 */
		inline void setText(std::string text) { this->m_config.text = std::move(text); }

		/**
		 * @brief Retrieves the current text content.
		 *
		 * @return a string containing the text content
		 */
		inline std::string getText() const { return this->m_config.text; }

		/**
		 * @brief Changes the text color.
		 *
		 * @param color the new text color
		 */
		inline void setColor(const glm::vec4 &color) { this->m_config.color = color; }

		/**
		 * @brief Retrieves the current text color.
		 *
		 * @return the text color
		 */
		inline glm::vec3 getColor() const { return this->m_config.color; }

		/**
		 * @brief Retrieves the current text position.
		 *
		 * @return the current text position
		 */
		inline glm::vec2 getPosition() const { return this->m_config.position; }

		/**
		 * @brief Changes the text position.
		 *
		 * @param pos the new text position
		 */
		inline void setPosition(const glm::vec2 &pos) { this->m_config.position = pos; }

		/**
		 * @brief Retrieves the current text scale.
		 *
		 * @return the text scale
		 */
		inline float getScale() const { return this->m_config.scale; }

		/**
		 * @brief Changes the text scale.
		 *
		 * @param color the new text scale
		 */
		inline void setScale(const float &scale) { this->m_config.scale = scale; }

		Text() = delete;

		/**
		 * @brief Instances a text using the given config.
		 *
		 * @param config the text config
		 */
		Text(const TextHelper &config) :
			m_config(config) {}

	private:
		/// text config
		TextHelper m_config{};

		/// text vertex array
		VertexArray m_vao{};
		/// text vertex buffer
		VertexBuffer m_vbo{};

		/// define if the text is created
		bool m_created = false;
	};

	/**
	 * @brief This class create a text manager in the current State.
	 * It inherit Layer class.
	 */
	class TextManager : public Layer {

		/**
		 * @brief Data structure to store single char data.
		 */
		struct Character {
			/// char texture id
			unsigned int textureId;
			/// char texture size
			glm::ivec2 size;
			/// char bearing
			glm::ivec2 bearing;
			/// char advance
			long long advance;
		};

	public:
		virtual void onAttach() override;

		virtual void onRender() override;

		/**
		 * @brief Creates and add the Text created from the given config.
		 *
		 * @param config the texture config
		 *
		 * @return the Shared<Text> object of the created text
		 */
		Shared<Text> addText(const TextHelper &config);

		TextManager(TextManager &other) = delete;

		void operator=(const TextManager &other) = delete;

		/**
		 * @brief Retrieves the instance of the TextManager.
		 * If it's not instanced, it will be instanced automatically.
		 *
		 * @return a Shared<TextManager> object
		 */
		inline static Shared<TextManager> instance() {
			if (s_pointer == nullptr) {
				Shared<TextManager> copy(new TextManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		virtual ~TextManager() override = default;

	private:
		TextManager() = default;

		/// static shared pointer for Singleton
		inline static Shared<TextManager> s_pointer = nullptr;

        /// data structure with all textures of each character
		std::map<unsigned long long, Character> m_characters{};
        /// vector with all Text
		std::vector<Shared<Text>> m_text{};

        /// text manager settings
		TextSettings m_settings{};
	};
} // namespace ogl

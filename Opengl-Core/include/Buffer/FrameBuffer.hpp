#pragma once

#include "Buffer.hpp"

#include "RenderBuffer.hpp"

#include "../Texture/Texture.hpp"

#include "../Core/Structs.hpp"

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL frame buffer object.
	 */
	class FrameBuffer : public Buffer {
	public:
		/**
		 * @brief Retrieves render buffer id.
		 *
		 * @return render buffer id
		 */
		inline unsigned int getRenderBufferId() const { return this->m_rbo.getId(); }

		/**
		 * @brief Sets the current render buffer with the given config.
		 *
		 * @param config render buffer config
		 *
		 * @note In this method a RenderBuffer is instanced.
		 */
		void setRenderBuffer(const RBConfig &config);

		/**
		 * @brief Sets the current render buffer.
		 *
		 * @param buffer the new RenderBuffer
		 */
		void setRenderBuffer(const RenderBuffer &buffer);

		/**
		 * @brief Retrieves the color texture id attached to the RenderBuffer.
		 *
		 * @return the texture id
		 */
		inline unsigned int getColorTexture() const { return this->m_texture.getId(); }

		/**
		 * @brief Sets the current RenderBuffer Texture.
		 */
		void setColorTexture(const Texture &texture);

		/**
		 * @brief Creates the frame buffer using his config.
		 *
		 * @return true if the frame buffer is created and valid
		 */
		bool createFrameBuffer();

		/**
		 * @brief Resize the frame buffer with the given size.
		 *
		 * @param width frame buffer width
		 * @param height frame buffer height
		 */
		void rescaleFrameBuffer(const unsigned int &width, const unsigned int &height);

		/**
		 * @brief Retrieves the frame buffer height.
		 *
		 * @return the frame buffer height
		 */
		inline float getHeight() const { return this->m_config.height; }

		/**
		 * @brief Retrieves the frame buffer width.
		 *
		 * @return the frame buffer width
		 */
		inline float getWidth() const { return this->m_config.width; }

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		FrameBuffer() = delete;

		/**
		 * @brief Instances a frame buffer with the given settings.
         *
         * @param config frame buffer configs
		 */
		FrameBuffer(const FBConfig &config) :
			m_config(config) {}
		~FrameBuffer() override;

	private:
		/// frame buffer configs
		FBConfig m_config{};
		/// render buffer
		RenderBuffer m_rbo{};
		/// render buffer texture
		Texture m_texture{};
	};
} // namespace ogl

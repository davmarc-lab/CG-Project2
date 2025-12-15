#pragma once

#include "Buffer.hpp"

#include "../Core/Structs.hpp"

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL render buffer object.
	 */
	class RenderBuffer : public Buffer {
	public:
		/**
		 * @brief Rescales the render buffer.
		 *
		 * @param width render buffer width
		 * @param height render buffer height
		 */
		void rescaleRenderBuffer(const unsigned int &width, const unsigned int &height);

		/**
		 * @brief Retreives the render buffer attachment type.
		 *
		 * @return render buffer attachment type
		 */
		inline unsigned int getAttachmentType() const { return this->m_config.attachment; }

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		RenderBuffer() = default;

		/**
		 * @brief Instances a render buffer with the given settings.
         *
         * @param config render buffer configs
		 */
		RenderBuffer(const RBConfig &config) : m_config(config) {}
		~RenderBuffer() override;

	private:
		RBConfig m_config{};
	};
} // namespace ogl

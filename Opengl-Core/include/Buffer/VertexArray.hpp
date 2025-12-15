#pragma once

#include "Buffer.hpp"

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL vertex array object.
	 */
	class VertexArray : public Buffer {
	public:
		// void linkVertexBuffer();

		/**
		 * @brief Links the given attribute with the given settings.
		 *
		 * @param layout attribute layout location
		 * @param size data size
		 * @param type data type
		 * @param normalize flag for normalized data
		 * @param stride data stride
		 * @param offset data offset
		 */
		void linkAttribFast(const unsigned int &layout, const int &size, const unsigned int &type,
							const bool &normalize, const int &stride, void *offset) const;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		/**
		 * @brief Instances basic vertex array object.
		 */
		VertexArray() = default;
		virtual ~VertexArray() override;
	};
} // namespace ogl

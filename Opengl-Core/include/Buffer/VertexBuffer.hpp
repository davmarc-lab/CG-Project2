#pragma once

#include <cstddef>
#include "Buffer.hpp"

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL vertex buffer object.
	 */
	class VertexBuffer : public Buffer {
	public:
		/**
		 * @brief Prepare and assign data to the vertex buffer object.
		 *
		 * @note It first binds the vertex buffer object.
		 *
		 * @tparam T vertex type
		 *
		 * @param vertices vector of T values
		 * @param size vertices size
		 * @param usage vertex buffer usage
		 */
		template <typename T>
		inline void setup(const T *vertices, const size_t &size, const unsigned int &usage) {
			this->bind();
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), vertices, usage);
		}

		/**
		 * @brief Prepare and assign float values to the vertex buffer object.
		 *
		 * @note It first binds the vertex buffer object.
		 *
		 * @param vertices vector of float values
		 * @param size vertices size
		 * @param usage vertex buffer usage
		 */
		inline void setup(const float *vertices, const size_t &size, const unsigned int &usage) {
			this->bind();
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, usage);
		}

		/**
		 * @brief Setups sub data of the vertex buffer object.
		 *
		 * @note It first binds the vertex buffer object.
		 *
		 * @warning This method is not implemented yet.
		 *
		 * @tparam T vertex type
		 *
		 * @param vertices vector of T values
		 * @param size vertices size
		 * @param offset data offset
		 */
		template <typename T>
		inline void setupSubData(const T *vertices, const size_t &size, const int &offset) {
			// TODO
		}

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		/**
		 * @brief Instances basic vertex buffer object.
		 */
		VertexBuffer() = default;
		~VertexBuffer() override;
	};
} // namespace ogl

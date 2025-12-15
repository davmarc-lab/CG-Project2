#pragma once

#include "Buffer.hpp"

#include <string>

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL uniform buffer object.
	 */
	class UniformBuffer : public Buffer {
	public:
		virtual void onAttach();
		virtual void onDetach();

		virtual void bind() const;
		virtual void unbind() const;

		/**
		 * @brief Prepare the uniform buffer layout with the given data.
		 *
		 * @note This method binds the current buffer.
		 *
		 * @param size data total size
		 * @param bindPoint shader binding point
		 * @param offset data offset
		 * @param rangeSize data range size
		 */
		void setup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize);

		/**
		 * @brief Prepare the uniform buffer layout with the given data without binding the buffer.
		 *
		 * @param size data total size
		 * @param bindPoint shader binding point
		 * @param offset data offset
		 * @param rangeSize data range size
		 */
		void fastSetup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize);

		/**
		 * @brief Updates the buffer data.
		 *
		 * @note This method binds the current buffer.
		 *
		 * @param offset data offset
		 * @param size data total size
		 * @param data pointer to the data
		 */
		void update(const unsigned int &offset, const unsigned int &size, void *data) const;

		/**
		 * @brief Updates the buffer data.
		 *
		 * @param offset data offset
		 * @param size data total size
		 * @param data pointer to the data
		 */
		void fastUpdate(const unsigned int &offset, const unsigned int &size, void *data) const;

		UniformBuffer() = default;

		~UniformBuffer();
	};
} // namespace ogl

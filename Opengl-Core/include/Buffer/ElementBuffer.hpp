#pragma once

#include <cstddef>
#include "Buffer.hpp"

namespace ogl {
	/**
	 * @brief This class provides methods to manage an OpenGL element buffer object.
	 */
	class ElementBuffer : public Buffer {
	public:
		/**
		 * @brief Setup the element buffer with the given data.
		 *
		 * @tparam T type of indices vector data
		 *
		 * @param indices indices vector
		 * @param size size of the vector
		 * @param usage buffer usage
		 */
		template <typename T>
		inline void setup(const T *indices, const size_t &size, const unsigned int &usage) const {
			this->bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * size, indices, usage);
		}

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		ElementBuffer() = default;
		~ElementBuffer() override;
	};
} // namespace ogl

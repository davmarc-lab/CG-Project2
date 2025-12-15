#pragma once

#include "../../include/Core/Structs.hpp"
#include "../../include/Core/Utils.hpp"

namespace ogl {

	/**
	 * @brief foo
	 */
	class Texture {
	public:
		/**
		 * Initialize the texture.
		 * This operation must be done before using this object.
		 */
		void onAttach();

		/**
		 * @brief Destroy the texture on the GPU.
		 */
		void onDetach();

		/**
		 * @brief Bind the current texture id.
		 */
		void bind() const;

		/**
		 * @brief Unbind the current texture id.
		 */
		void unbind() const;

		/**
		 * @brief Set an `int` parameter for the current texture in bound.
		 *
		 * @param pname Parameter name.
		 * @param pval Parameter `int` value.
		 */
		void setTexParameteri(const unsigned int &pname, const int &pval);

		/**
		 * @brief Set a `float` parameter of the current texture in bound.
		 *
		 * @param pname Parameter name.
		 * @param pval Parameter `float` value.
		 */
		void setTexParameterf(const unsigned int &pname, const float &pval);

		/**
		 * @brief Bind the `Texture` id and calls `fastCreatetexture2D`.
		 *
		 * @param *data Pointer to texture's data.
		 */
		inline void createTexture2D(void *data) {
			this->bind();
			this->fastCreateTexture2D(data);
		}

		/**
		 * @brief Creates the texture already binded.
		 *
		 * @param *data Pointer to texture's data.
		 */
		void fastCreateTexture2D(void *data);

		/**
		 * @brief Creates custom texture 2D with the given parameters and data.
		 *
		 * @param target the texture binding target
		 * @param level the mipmap level of the texture.
		 * @param internalFormat the texture internalFormat value
		 * @param width the texture width
		 * @param height the texture height
		 * @param border the texture border (usually `0`)
		 * @param format the texture pixel format
		 * @param type the type of texture data
		 * @param data the texture data
		 */
		void fastCreateCustomTexture2D(const unsigned int &target, const unsigned int &level, const unsigned int &internalFormat, const unsigned int &width, const unsigned int &height, const unsigned int &border, const unsigned int &format, const unsigned int &type, void *data);

		/**
		 * @brief Generates the binded texture mipmap, only if the texture is created.
		 */
		void generateMipmap();

		/**
		 * @brief Retrieves the texture id.
		 *
		 * @return texture id.
		 */
		inline unsigned int getId() const { return this->m_id; }

		/**
		 * @brief Retrieves the texture width.
		 *
		 * @return texture width.
		 */
		inline unsigned int getWidth() const { return this->m_size.x; }

		/**
		 * @brief Changes texture width.
		 *
		 * @param width Texture width.
		 */
		inline void setWidth(const unsigned int &width) {
			this->m_size.x = width;
			this->rescaleTexture();
		}

		/**
		 * @brief Retrieves the texture height.
		 *
		 * @return texture height.
		 */
		inline unsigned int getHeight() const { return this->m_size.y; }

		/**
		 * @brief Changes texture height.
		 *
		 * @param height Texture height.
		 */
		inline void setHeight(const unsigned int &height) {
			this->m_size.y = height;
			this->rescaleTexture();
		}

		/**
		 * @brief Retrieves the texture size.
		 *
		 * @return texture size.
		 */
		inline Pair<unsigned int> getSize() const { return this->m_size; }

		/**
		 * @brief Updates the size of the texture.
		 *
		 * @param size the texture new size
		 * @see Texture::rescaleTexture()
		 */
		inline void setSize(const Pair<unsigned int> &size) {
			this->m_size = size;
			this->rescaleTexture();
		}

		Texture() = default;

		/**
		 * @brief Instances a Texture with the given params and size.
         *
         * @param params the texture params data
         * @param size the texture size
		 */
		Texture(const TextureParams &params, const Pair<unsigned int> &size) :
			m_params(params), m_size(size) {
		}

		/**
		 * @brief Instances a Texture with the given params.
         *
         * It's the same as Texture::Texture(const TextureParams &params, const Pair<unsigned int> &size)
         *
         * @param params the texture params data
         * @param width the texture width in pixel
         * @param height the texture height in pixel
		 */
		Texture(const TextureParams &params, const unsigned int &width, const unsigned int &height) :
			m_params(params), m_size({width, height}) {
		}

		~Texture() = default;

	private:
		/// Rescales the texture with new width and height.
		void rescaleTexture();

		unsigned int m_id = 0;

		TextureParams m_params{};

		Pair<unsigned int> m_size{};

		void *m_data = NULL;

		bool m_attached = false;
		bool m_created = false;
	};

} // namespace ogl

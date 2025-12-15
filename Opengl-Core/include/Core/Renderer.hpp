#pragma once

#include "../Buffer/ElementBuffer.hpp"
#include "../Buffer/VertexArray.hpp"
#include "../Buffer/VertexBuffer.hpp"

#include "../Shader/Shader.hpp"

#include "Utils.hpp"

#include <cstddef>
#include <vector>

namespace ogl {

	/**
	 * @brief Data structure to store primitives data.
	 */
	struct Primitive {
		/// primitive vao
		VertexArray vao{};
		/// primitive geometry vbo
		VertexBuffer vbog{};
		/// primitive vertices vector
		std::vector<glm::vec3> vertex{};
		/// primitive color vbo
		VertexBuffer vboc{};
		/// primitive colors vector
		std::vector<glm::vec4> colors{};
		/// primitive element buffer
		ElementBuffer ebo{};
		/// primitive indices vector
		std::vector<unsigned int> index{};
		/// primitive texture vbo
		VertexBuffer vbot{};
		/// primitive texture coords
		std::vector<glm::vec2> texCoords{};
		/// primitive normal vectors vbo
		VertexBuffer vbon{};
		/// primitive normal vector
		std::vector<glm::vec3> normals{};

		/// primitive ids for EntityManager
		std::vector<unsigned int> ids{};
		/// primitive vao
		VertexBuffer vbomo{};
		/// vector with all primitives model matrix
		std::vector<glm::mat4> modelOffset{};
		/// primitive vao
		VertexBuffer vboco{};
		/// vector with all primitives colors
		std::vector<glm::vec4> colorOffset{};

		/**
		 * @brief Clears primitive buffers.
		 */
		inline void clear() {
			this->vbog.onDetach();
			this->vboc.onDetach();
			this->vbot.onDetach();
			this->vbon.onDetach();
			this->vbomo.onDetach();
			this->vboco.onDetach();
			this->ebo.onDetach();
			this->vao.onDetach();
		}
	};

	/**
	 * @brief Data structure used to manage renderer statistics.
	 *
	 * @note It's used for rendering instanced mesh.
	 */
	struct RendererStats {
		/// renderer draw calls
		int drawCalls = 0;
		/// renderer cubes number
		unsigned int numCubes = 0;
		/// renderer pyramids number
		unsigned int numPyramids = 0;
		/// renderer spheres number
		unsigned int numSpheres = 0;
		/// renderer thorus number
		unsigned int numThorus = 0;
	};

	/**
	 * @enum RenderPrimitiveType
	 * @brief Specifies different primitives type.
	 */
	enum RenderPrimitiveType {
		PRIMITIVE_CUBE,
		PRIMITIVE_SPHERE,
		PRIMITIVE_PYRAMID,
		PRIMITIVE_THORUS,
	};

	/**
	 * @brief This singleton class manages all OpenGL render calls.
	 */
	class Renderer {
	public:
		/**
		 * @brief Initialize renderer primitives and ShaderProgram.
		 */
		void init();

		/**
		 * @brief Clears renderer primitives and ShaderProgram.
		 */
		void clear();

		/**
		 * @brief Appends a sphere to the current scene.
		 *
		 * @param id sphere ecs id
		 * @param model sphere model matrix
		 * @param color sphere color
		 */
		void appendSphere(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);

		/**
		 * @brief Appends a cube to the current scene.
		 *
		 * @param id sphere ecs id
		 * @param model sphere model matrix
		 * @param color sphere color
		 */
		void appendCube(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);

		/**
		 * @brief Appends a pyramid to the current scene.
		 *
		 * @param id sphere ecs id
		 * @param model sphere model matrix
		 * @param color sphere color
		 */
		void appendPyramid(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);

		/**
		 * @brief Appends a thorus to the current scene.
		 *
		 * @param id sphere ecs id
		 * @param model sphere model matrix
		 * @param color sphere color
		 */
		void appendThorus(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);

		/**
		 * @brief Calls OpenGL render arrays method with the given parameters.
		 *
		 * @param vao mesh vao id
		 * @param mode draw method
		 * @param first index of first vertex buffer vertex
		 * @param size size of geometry vertex buffer data
		 */
		void drawArrays(const unsigned int &vao, const unsigned int &mode, const int &first, const size_t &size);

		/**
		 * @brief Calls OpenGL render elements method with the given parameters.
		 *
		 * @param vao mesh vao id
		 * @param mode draw method
		 * @param size size of geometry vertex buffer data
		 * @param type indices data type
		 * @param indices indices vector (by default uses element buffer)
		 */
		void drawElements(const unsigned int &vao, const unsigned int &mode, const size_t &size, const unsigned int &type, const void *indices = 0);

		/**
		 * @brief Renders all instanced mesh appended to the scene.
		 */
		void drawAllInstanced();

		/**
		 * @brief Prepares all primitives buffers.
		 *
		 * @warning This method doesn't work well yet: at the moment it's working only
		 * for spheres primitives.
		 *
		 * @param models all model matrix
		 * @param colors all primitives colors
		 */
		void prepareBuffers(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors);

		/**
		 * @brief Retrieves all renderer statistics.
		 *
		 * @return RenderStats object
		 */
		RendererStats getRendererStats() const { return this->m_stats; }

		/**
		 * @brief Resets al renderer statistics
		 */
		void resetDrawStats() {
			this->m_stats.numCubes = 0;
			this->m_stats.numPyramids = 0;
			this->m_stats.numSpheres = 0;
			this->m_stats.numThorus = 0;
		}

		Renderer(Renderer &other) = delete;

		void operator=(const Renderer &other) = delete;

		/**
		 * @brief Retrieves the instance of the Renderer.
		 * If it's not instanced, it will be instanced automatically.
		 *
		 * @return a Shared<Renderer> object
		 */
		inline static Shared<Renderer> instance() {
			if (s_pointer == nullptr) {
				Shared<Renderer> copy(new Renderer());
				copy.swap(s_pointer);
			}
			return s_pointer;
		}

		~Renderer() = default;

	private:
		Renderer() = default;

		/// static shared pointer for Singleton
		inline static Shared<Renderer> s_pointer = nullptr;

		/// renderer stats
		RendererStats m_stats{};

		/// cube primitive
		Primitive m_cube{};
		/// pyramid primitive
		Primitive m_pyramid{};
		/// sphere primitive
		Primitive m_sphere{};
		/// thorus primitive
		Primitive m_thorus{};
		/// renderer init flag
		bool m_init = false;
	};
} // namespace ogl

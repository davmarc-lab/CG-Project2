#pragma once

#include "../Core/Resource.hpp"
#include "../Graphic.hpp"

#include <iostream>

namespace ogl {
	/**
	 * @namespace shader
	 * @brief Contains all shader utilities
	 */
	namespace shader {
		/// default directory for shader programs
		const std::string DEFAULT_SHADER_DIR = "shader/";
		/// default location for shader program resources
		const std::string DEFAULT_SHADER_LOCATION = res::DEFAULT_LOCATION + DEFAULT_SHADER_DIR;

		/**
		 * @enum ShaderType
		 * @brief Defines all type of ShaderProgram
		 */
		enum ShaderType {
			SHADER_TYPE_VERTEX,
			SHADER_TYPE_FRAGMENT,
			SHADER_TYPE_GEOMETRY,
			SHADER_TYPE_PROGRAM,
			SHADER_TYPE_NONE,
		};

		/**
		 * @brief Retrieves the string from the given ShaderType.
		 *
		 * @param type the shader type
		 *
		 * @return the ShaderType string
		 */
		inline std::string getShaderTypeString(shader::ShaderType type) {
			switch (type) {
				case SHADER_TYPE_VERTEX:
					return "VERTEX";
				case shader::SHADER_TYPE_FRAGMENT:
					return "FRAGMENT";
				case shader::SHADER_TYPE_GEOMETRY:
					return "GEOMETRY";
				case shader::SHADER_TYPE_PROGRAM:
					return "PROGRAM";
				default:
					return "NONE";
			}
		}

		/**
		 * @brief Retrieves the glfw shader type value from the given ShaderType.
		 *
		 * @param type the shader type
		 *
		 * @return the glfw value of the ShaderType
		 */
		inline unsigned int getShaderTypeValue(shader::ShaderType type) {
			switch (type) {
				case shader::SHADER_TYPE_VERTEX:
					return GL_VERTEX_SHADER;
				case shader::SHADER_TYPE_FRAGMENT:
					return GL_FRAGMENT_SHADER;
				case shader::SHADER_TYPE_GEOMETRY:
					return GL_GEOMETRY_SHADER;
				case shader::SHADER_TYPE_PROGRAM:
					std::cerr << "Invalid Shader type -> " << getShaderTypeString(type) << ".\nUse a ShaderProgram instead.";
					assert(false);
				default: {
					std::cerr << "Invalid Shader type -> " << getShaderTypeString(type) << ".\n";
					assert(false);
				}
			}
		}
	} // namespace shader

	/**
	 * @brief This class provides methods to manage OpenGL shaders.
	 */
	class Shader {
	public:
		/**
		 * @brief Creates the shader resource and instances the shader.
		 *
		 * @warning The OpenGL context should be already initialized (glad or glew).
		 */
		void createShader();

		/**
		 * @brief Retrieves the shader id.
		 *
		 * @return the shader id
		 */
		inline unsigned int getId() const { return this->m_id; }

		Shader() = delete;

		/**
		 * @brief Instances a shader of the type given and builds the path.
		 *
		 * @param location the directory path
		 * @param file the shader file in location
		 * @param type the shader type
		 */
		Shader(const std::string &location, const std::string &file, const shader::ShaderType &type) :
			m_location(std::move(location)), m_file(std::move(file)), m_type(type) {}

		/**
		 * @brief Instances a shader of the type given.
		 *
		 * @param file the shader file in the default location
		 * @param type the shader type
		 */
		Shader(const std::string &file, const shader::ShaderType &type) :
			Shader(shader::DEFAULT_SHADER_LOCATION, file, type) {}

		~Shader() = default;

	private:
		/**
		 * @brief Check compile errors after compiling the shader.
		 */
		void checkCompileErrors();

		/// shader id
		unsigned int m_id = 0;
		/// shader type
		shader::ShaderType m_type = shader::ShaderType::SHADER_TYPE_NONE;

		/// shader file directory path
		std::string m_location{};
		/// shader file name
		std::string m_file{};
		/// shader resource id
		unsigned int m_resourceId = 0;
	};

	/**
	 * @brief This class manages an OpenGL shader program.
	 */
	class ShaderProgram {
	public:
		/**
		 * @brief Creates and compile the shader program.
		 *
		 * @warning The OpenGL context should be already initialized (glad or glew).
		 */
		void createShaderProgram();

		/**
		 * @brief Binds the shader program.
		 */
		void use() const;

		/**
		 * @brief Deletes the shader program.
		 */
		void clear();

		/**
		 * @brief Retrieves the shader program id.
		 */
		inline unsigned int getId() const { return this->m_id; }

		/**
		 * @brief Sets an uniform bool variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setBool(const std::string &name, bool value) const;
		/**
		 * @brief Sets an uniform int variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setInt(const std::string &name, int value) const;
		/**
		 * @brief Sets an uniform float variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setFloat(const std::string &name, float value) const;

		/**
		 * @brief Sets an uniform float variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setVec2(const std::string &name, const glm::vec2 &value) const;
		/**
		 * @brief Sets an uniform vec2 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param x the vector x coordinate in the program
		 * @param y the vector y coordinate in the program
		 */
		void setVec2(const std::string &name, float x, float y) const;

		/**
		 * @brief Sets an uniform vec3 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setVec3(const std::string &name, const glm::vec3 &value) const;
		/**
		 * @brief Sets an uniform vec3 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param x the vector x coordinate in the program
		 * @param y the vector y coordinate in the program
		 * @param z the vector z coordinate in the program
		 */
		void setVec3(const std::string &name, float x, float y, float z) const;

		/**
		 * @brief Sets an uniform vec4 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param value the variable value in the program
		 */
		void setVec4(const std::string &name, const glm::vec4 &value) const;
		/**
		 * @brief Sets an uniform vec4 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param x the vector x coordinate in the program
		 * @param y the vector y coordinate in the program
		 * @param z the vector z coordinate in the program
		 * @param w the vector w coordinate in the program
		 */
		void setVec4(const std::string &name, float x, float y, float z, float w) const;

		/**
		 * @brief Sets an uniform mat2 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param mat the variable value in the program
		 */
		void setMat2(const std::string &name, const glm::mat2 &mat) const;
		/**
		 * @brief Sets an uniform mat3 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param mat the variable value in the program
		 */
		void setMat3(const std::string &name, const glm::mat3 &mat) const;
		/**
		 * @brief Sets an uniform mat4 variable in the shader program.
		 *
		 * @param name the name in the program
		 * @param mat the variable value in the program
		 */
		void setMat4(const std::string &name, const glm::mat4 &mat) const;

		ShaderProgram() = delete;

		/**
		 * @brief Instances a shader program with vertex and fragment shader.
		 *
		 * @param vloc the vertex shader file directory path
		 * @param vfile the vertex shader file in vloc
		 * @param floc the fragment shader file directory path
		 * @param ffile the fragment shader file in vloc
		 */
		ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile);

		/**
		 * @brief Instances a shader program with vertex and fragment shader.
		 *
		 * @param vfile the vertex shader file in the default shader resource directory
		 * @param ffile the fragment shader file in the default shader resource directory
		 */
		ShaderProgram(const std::string &vfile, const std::string &ffile) :
			ShaderProgram(shader::DEFAULT_SHADER_LOCATION, vfile,
						  shader::DEFAULT_SHADER_LOCATION, ffile) {}

		/**
		 * @brief Instances a shader program with vertex, fragment and geometry shader.
		 *
		 * @param vloc the vertex shader file directory path
		 * @param vfile the vertex shader file in vloc
		 * @param floc the fragment shader file directory path
		 * @param ffile the fragment shader file in vloc
		 * @param gloc the geometry shader file directory path
		 * @param gfile the geometry shader file in vloc
		 */
		ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile,
					  const std::string &gloc, const std::string &gfile);

		/**
		 * @brief Instances a shader program with vertex and fragment shader.
		 *
		 * @param vfile the vertex shader file in the default shader resource directory
		 * @param ffile the fragment shader file in the default shader resource directory
		 * @param gfile the geometry shader file in the default shader resource directory
		 */
		ShaderProgram(const std::string &vfile, const std::string &ffile, const std::string &gfile) :
			ShaderProgram(shader::DEFAULT_SHADER_LOCATION, vfile,
						  shader::DEFAULT_SHADER_LOCATION, ffile,
						  shader::DEFAULT_SHADER_LOCATION, gfile) {}

		~ShaderProgram();

	private:
		/**
		 * @brief Checks if there were errors during Shader linking while creating
		 * the shader program.
		 */
		void checkLinkingErrors();

		/// shader program id
		unsigned int m_id = 0;
		/// shader program vertex shader
		Unique<Shader> m_vert = nullptr;
		/// shader program fragment shader
		Unique<Shader> m_frag = nullptr;
		/// shader program geometry shader
		Unique<Shader> m_geom = nullptr;
	};
} // namespace ogl

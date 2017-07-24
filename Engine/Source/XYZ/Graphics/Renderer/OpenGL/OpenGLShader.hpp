//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Graphics/Shader/ShaderProgram.hpp"
#include "XYZ/Graphics/Shader/ShaderSource.hpp"
#include "OpenGLBuffer.hpp"
#include <GL/glew.h>

#include <string>
#include <unordered_map>

namespace XYZ::Graphics::Renderer::OpenGL {

	/**
	 * A OpenGL shader
	 */
	template<typename ShaderType>
	class OpenGLShader final : public ShaderType {
	public:
		/**
		 * The OpenGL shader ID
		 */
		GLuint shaderID;

	public:
		/**
		 * Creates a new empty shader
		 */
		OpenGLShader();

		/**
		 * Create a new shader from the shader source object
		 *
		 * @param source the shader source to compile
		 */
		explicit OpenGLShader(const Shader::ShaderSource& source);

		/**
		 * Creates a new shader object from a already existing OpenGL shader handle
		 *
		 * @param shaderID the OpenGL shader handle
		 */
		OpenGLShader(GLuint shaderID);

		/**
		 * Deleted copy constructor
		 *
		 * @param other the shader to copy from
		 */
		OpenGLShader(const OpenGLShader& other) = delete;

		/**
		 * Deleted copy assignment operator
		 *
		 * @param other the shader to copy from
		 *
		 * @return this
		 */
		OpenGLShader& operator=(const OpenGLShader& other) = delete;

		/**
		 * Move constructor
		 *
		 * @param other the shader to move from
		 */
		OpenGLShader(OpenGLShader&& other);

		/**
		 * Move assignment operator
		 *
		 * @param other the shader to move from
		 *
		 * @return this
		 */
		OpenGLShader& operator=(OpenGLShader&& other);

		/**
		 * Destroys the shader.
		 */
		~OpenGLShader();

	public:
		/**
		 * @return the shader binary
		 */
		virtual Shader::ShaderBinary getShaderBinary() const final;
	};

	/**
	 * A OpenGL vertex shader
	 */
	using OpenGLVertexShader = OpenGLShader<Shader::VertexShader>;

	/**
	 * A OpenGL fragment shader
	 */
	using OpenGLFragmentShader = OpenGLShader<Shader::FragmentShader>;

	/**
	 * A OpenGL geometry shader
	 */
	using OpenGLGeometryShader = OpenGLShader<Shader::GeometryShader>;

	/**
	 * A fully compiled and linked OpenGL shader program
	 */
	class OpenGLShaderProgram final : public Shader::ShaderProgram {
	public:
		/**
		 * The OpenGL shader program ID
		 */
		GLuint programID;

		/**
		 * A cache of all uniform variables indexes for this shader program
		 */
		std::unordered_map<std::string, GLuint> uniforms;

	public:
		/**
		 * Creates a new empty shader program.
		 */
		OpenGLShaderProgram();

		/**
		 * Creates a new shader program with a vertex, fragment and geometry shaders.
		 *
		 * @param vertexShader the vertex shader to link
		 * @param fragmentShader the fragment shader to link
		 * @param geometryShader the geometry shader to link
		 */
		OpenGLShaderProgram(const OpenGLVertexShader& vertexShader,
							const OpenGLFragmentShader& fragmentShader,
							const OpenGLGeometryShader& geometryShader = OpenGLGeometryShader());

		/**
		 * Creates a new shader program from a precompiled shader binary
		 *
		 * @param binary the shader binary
		 */
		OpenGLShaderProgram(const Shader::ShaderBinary& binary);

		/**
		 * Deleted copy constructor
		 *
		 * @param other the shader program to copy from
		 */
		OpenGLShaderProgram(const OpenGLShaderProgram& other) = delete;

		/**
		 * Deleted copy assignment operator
		 *
		 * @param other the shader program to copy from
		 *
		 * @return this
		 */
		OpenGLShaderProgram& operator=(const OpenGLShaderProgram& other) = delete;

		/**
		 * Move constructor
		 *
		 * @param other the shader program to move from
		 */
		OpenGLShaderProgram(OpenGLShaderProgram&& other);

		/**
		 * Move assignment operator
		 *
		 * @param other the shader program to move from
		 *
		 * @return this
		 */
		OpenGLShaderProgram& operator=(OpenGLShaderProgram&& other);

		/**
		 * Destroys the shader
		 */
		~OpenGLShaderProgram() override;

	public:
		/**
		 * Activates the shader
		 */
		virtual void activate() override;

		/**
		 * Deactivates the shader
		 */
		virtual void deactivate() override;

	public:
		/**
		 * Sets a boolean uniform variable
		 *
		 * @param name the uniform name
		 * @param b the uniform value
		 */
		virtual void set(const std::string& name, bool b) override;

		/**
		 * Sets a float uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, float v) override;

		/**
		 * Sets a int uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, int v) override;

		/**
		 * Sets a unsigned int uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, unsigned int v) override;

		/**
		 * Sets a vec2 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::vec2 v) override;

		/**
		 * Sets a vec3 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::vec3 v) override;

		/**
		 * Sets a vec4 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::vec4 v) override;

		/**
		 * Sets a mat2 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::mat2 v) override;

		/**
		 * Sets a mat3 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::mat3 v) override;

		/**
		 * Sets a mat4 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
		virtual void set(const std::string& name, glm::mat4 v) override;

		/**
		 * Sets a uniform buffer
		 *
		 * @tparam T the uniform buffer type
		 *
		 * @param name the uniform buffer name
		 * @param position the uniform buffer position
		 * @param uniformBuffer the uniform buffer
		 */
		template<typename T>
		void set(const std::string& name, unsigned int position, const OpenGLUniformBuffer<T>& uniformBuffer) {
			unsigned int index = glGetUniformBlockIndex(programID, "ViewProjection");
			glUniformBlockBinding(programID, index, position);
			glBindBufferRange(GL_UNIFORM_BUFFER, position, uniformBuffer, 0, sizeof(T));
		}

	public:
		/**
		 * @return the shader binary
		 */
		virtual Shader::ShaderBinary getShaderBinary() const final;

	private:
		/**
		 * Gets a cached uniform location. If the location is not on cache,
		 * will fetch a new value from OpenGL and store in the local cache.
		 *
		 * @param name the uniform name
		 *
		 * @return the uniform location as returned by
		 * <tt>glGetUniformLocation</tt>
		 */
		GLuint getUniformLocation(const std::string& name);

	};

}



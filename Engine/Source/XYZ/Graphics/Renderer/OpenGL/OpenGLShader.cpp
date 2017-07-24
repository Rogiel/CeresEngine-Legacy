//
// Created by Rogiel Sulzbach on 7/8/17.
//

#include "OpenGLShader.hpp"

#include "OpenGLTexture.hpp"

#include <GL/glew.h>
#include <string>
#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	template<typename ShaderType>
	const GLenum OpenGLShaderType = 0;

	template<> const GLenum OpenGLShaderType<Shader::VertexShader> = GL_VERTEX_SHADER;
	template<> const GLenum OpenGLShaderType<Shader::FragmentShader> = GL_FRAGMENT_SHADER;
	template<> const GLenum OpenGLShaderType<Shader::GeometryShader> = GL_GEOMETRY_SHADER;

	// -----------------------------------------------------------------------------------------------------------------

	template<typename ShaderType>
	OpenGLShader<ShaderType>::OpenGLShader() :
			shaderID(0) {}

	template<typename ShaderType>
	OpenGLShader<ShaderType>::OpenGLShader(const Graphics::Shader::ShaderSource& source) {
		// Compile Vertex Shader
		shaderID = glCreateShader(OpenGLShaderType<ShaderType>);

		char const* sourcePointer = source.getSource().c_str();
		glShaderSource(shaderID, 1, &sourcePointer, nullptr);
		glCompileShader(shaderID);

		// Check Vertex Shader
		GLint result = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		int infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {
			std::string errorMessage;
			errorMessage.resize((unsigned int) infoLogLength - 1);

			glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &errorMessage[0]);
			std::cout << errorMessage << std::endl;
		}
	}

	template<typename ShaderType>
	OpenGLShader<ShaderType>::OpenGLShader(GLuint shaderID) :
			shaderID(shaderID) {}

	template<typename ShaderType>
	OpenGLShader<ShaderType>::OpenGLShader(OpenGLShader&& other) {
		shaderID = other.shaderID;
		other.shaderID = 0;
	}

	template<typename ShaderType>
	OpenGLShader<ShaderType>& OpenGLShader<ShaderType>::operator=(OpenGLShader&& other) {
		shaderID = other.shaderID;
		other.shaderID = 0;
		return *this;
	}

	template<typename ShaderType>
	OpenGLShader<ShaderType>::~OpenGLShader() {
		if(shaderID != 0) {
			glDeleteShader(shaderID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	template<typename ShaderType>
	Shader::ShaderBinary OpenGLShader<ShaderType>::getShaderBinary() const {
		return {};
	}

	// -----------------------------------------------------------------------------------------------------------------

	template
	class OpenGLShader<Shader::VertexShader>;

	template
	class OpenGLShader<Shader::FragmentShader>;

	template
	class OpenGLShader<Shader::GeometryShader>;

	// -----------------------------------------------------------------------------------------------------------------

	OpenGLShaderProgram::OpenGLShaderProgram() :
			programID(0) {}

	OpenGLShaderProgram::OpenGLShaderProgram(const OpenGLVertexShader& vertexShader,
											 const OpenGLFragmentShader& fragmentShader,
											 const OpenGLGeometryShader& geometryShader) {
		programID = glCreateProgram();

		glAttachShader(programID, vertexShader.shaderID);
		glAttachShader(programID, fragmentShader.shaderID);
		if(geometryShader.shaderID != 0) {
			glAttachShader(programID, geometryShader.shaderID);
		}
		glLinkProgram(programID);

		// Check the program
		GLint result = GL_FALSE;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);

		int infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {
			std::string errorMessage;
			errorMessage.resize((unsigned int) infoLogLength - 1);

			glGetProgramInfoLog(programID, infoLogLength, nullptr, &errorMessage[0]);
			std::cout << errorMessage << std::endl;
		}

		glDetachShader(programID, vertexShader.shaderID);
		glDetachShader(programID, fragmentShader.shaderID);
		if(geometryShader.shaderID != 0) {
			glDetachShader(programID, geometryShader.shaderID);
		}
	}

	OpenGLShaderProgram::OpenGLShaderProgram(const Shader::ShaderBinary& binary) {
		programID = glCreateProgram();
		glProgramBinary(programID, *((GLenum*) binary.getBinary().data()), binary.getBinary().data(),
						static_cast<GLint>(binary.getBinary().size() - sizeof(GLenum)));
	}

	OpenGLShaderProgram::OpenGLShaderProgram(OpenGLShaderProgram&& other) {
		programID = other.programID;
		other.programID = 0;
	}

	OpenGLShaderProgram& OpenGLShaderProgram::operator=(OpenGLShaderProgram&& other) {
		programID = other.programID;
		other.programID = 0;
		return *this;
	}

	OpenGLShaderProgram::~OpenGLShaderProgram() {
		if(programID != 0) {
			glDeleteProgram(programID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLShaderProgram::activate() {
		glUseProgram(programID);
	}

	void OpenGLShaderProgram::deactivate() {
		glUseProgram(0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLShaderProgram::set(const std::string& name, bool b) {
		glUniform1i(
				getUniformLocation(name),
				b
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, float v) {
		glUniform1f(
				getUniformLocation(name),
				v
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, int i) {
		glUniform1i(
				getUniformLocation(name),
				i
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, unsigned int i) {
		glUniform1i(
				getUniformLocation(name),
				i
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::vec2 v) {
		glUniform2f(
				getUniformLocation(name),
				v[0], v[1]
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::vec3 v) {
		glUniform3f(
				getUniformLocation(name),
				v[0], v[1], v[2]
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::vec4 v) {
		glUniform4f(
				getUniformLocation(name),
				v[0], v[1], v[2], v[3]
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::mat2 v) {
		glUniformMatrix2fv(
				getUniformLocation(name),
				1, GL_FALSE, &v[0][0]
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::mat3 v) {
		glUniformMatrix3fv(
				getUniformLocation(name),
				1, GL_FALSE, &v[0][0]
		);
	}

	void OpenGLShaderProgram::set(const std::string& name, glm::mat4 v) {
		glUniformMatrix4fv(
				getUniformLocation(name),
				1, GL_FALSE, &v[0][0]
		);
	}

	// -----------------------------------------------------------------------------------------------------------------

	GLuint OpenGLShaderProgram::getUniformLocation(const std::string& name) {
		auto found = uniforms.find(name);
		if(found == uniforms.end()) {
			found = uniforms.insert(std::make_pair(name, glGetUniformLocation(programID, name.c_str()))).first;
		}
		return found->second;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Shader::ShaderBinary OpenGLShaderProgram::getShaderBinary() const {
		std::vector<char> binary;

		GLint size = 0;
		glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &size);
		if(size == 0) {
			return {};
		}

		binary.resize(size + sizeof(GLenum));
		glGetProgramBinary(programID, size, nullptr, (GLenum*) binary.data(), binary.data() + sizeof(GLenum));

		return Shader::ShaderBinary(std::move(binary));
	}

}
//
// Created by Rogiel Sulzbach on 7/8/17.
//

#include "OpenGLCompiler.hpp"

#include <GL/glew.h>

#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLVertexBuffer.hpp"

#include <iostream>
#include <XYZ/Graphics/Material/PhongMaterial.hpp>

namespace XYZ::Graphics::Renderer::OpenGL {

	Shader::VertexShader::Ptr OpenGLCompiler::compileVertexShader(
			const Shader::ShaderSource& shaderSource) {
		return std::make_shared<OpenGLVertexShader>(
				shaderSource
		);
	}

	Shader::FragmentShader::Ptr OpenGLCompiler::compileFragmentShader(
			const Shader::ShaderSource& shaderSource) {
		return std::make_shared<OpenGLFragmentShader>(
				shaderSource
		);
	}

	Shader::GeometryShader::Ptr OpenGLCompiler::compileGeometryShader(
			const Shader::ShaderSource& shaderSource) {
		return std::make_shared<OpenGLGeometryShader>(
				shaderSource
		);
	}

	std::unique_ptr<Shader::ShaderProgram> OpenGLCompiler::compileShaderProgram(
			const Shader::VertexShader::Ptr& vertexShader,
			const Shader::FragmentShader::Ptr& fragmentShader,
			const Shader::GeometryShader::Ptr& geometryShader) {
		if(geometryShader == nullptr) {
			return std::make_unique<OpenGLShaderProgram>(
					*static_cast<OpenGLVertexShader*>(vertexShader.get()),
					*static_cast<OpenGLFragmentShader*>(fragmentShader.get())
			);
		}
		return std::make_unique<OpenGLShaderProgram>(
				*static_cast<OpenGLVertexShader*>(vertexShader.get()),
				*static_cast<OpenGLFragmentShader*>(fragmentShader.get()),
				*static_cast<OpenGLGeometryShader*>(geometryShader.get())
		);
	}

	Texture::Texture::Ptr OpenGLCompiler::compileTexture(
			const Texture::TextureImage& textureImage) {
		if(textureImage.getCompiledTexture() != nullptr) {
			return textureImage.getCompiledTexture();
		}

		auto compiled = std::make_shared<OpenGLTexture>(
				textureImage
		);

		const_cast<Texture::TextureImage&>(textureImage).setCompiledTexture(compiled);
		return compiled;
	}

	std::shared_ptr<VertexBuffer> OpenGLCompiler::compileMesh(
			const Mesh::Mesh& mesh) {
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// The indices buffer
		auto& indices = mesh.getIndices();
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		auto& vertices = mesh.getVertices();
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Mesh::Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
				0,                                           // attribute
				3,                                           // size
				GL_FLOAT,                                    // type
				GL_FALSE,                                    // normalized?
				sizeof(Mesh::Vertex),                        // stride
				(void*) offsetof(Mesh::Vertex, position)     // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
				1,                                          // attribute
				2,                                          // size
				GL_FLOAT,                                   // type
				GL_FALSE,                                   // normalized?
				sizeof(Mesh::Vertex),                       // stride
				(void*) offsetof(Mesh::Vertex, texCoords)   // array buffer offset
		);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
				2,                                          // attribute
				3,                                          // size
				GL_FLOAT,                                   // type
				GL_FALSE,                                   // normalized?
				sizeof(Mesh::Vertex),                       // stride
				(void*) offsetof(Mesh::Vertex, normal)      // array buffer offset
		);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(
				3,                                          // attribute
				3,                                          // size
				GL_FLOAT,                                   // type
				GL_FALSE,                                   // normalized?
				sizeof(Mesh::Vertex),                       // stride
				(void*) offsetof(Mesh::Vertex, tangent)     // array buffer offset
		);

		glBindVertexArray(0);

		// create a vertex buffer object and return it
		return std::make_shared<OpenGLVertexBuffer>(
				ebo, vertexBuffer, vertexArrayID, static_cast<GLsizei>(indices.size())
		);

	}

	// -----------------------------------------------------------------------------------------------------------------

	Material::PhongMaterial::Ptr OpenGLCompiler::createPhongMaterial() {
		return nullptr;
	}


}
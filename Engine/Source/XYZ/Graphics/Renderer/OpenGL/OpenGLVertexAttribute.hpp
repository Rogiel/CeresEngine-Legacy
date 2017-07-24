//
// Created by Rogiel Sulzbach on 7/21/17.
//

#pragma once

#include <GL/glew.h>

#include <memory>

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLVertexAttribute {
	private:
		/**
		 * The Vertex Attributes Object (VAO) object
		 */
		GLuint vao;

	public:
		/**
		 * Creates a new vertex attributes object
		 */
		OpenGLVertexAttribute() {
			glGenVertexArrays(1, &vao);
		}

		/**
		 * Creates a null vertex attributes object
		 */
		OpenGLVertexAttribute(std::nullptr_t) : vao(0) { }

		/**
		 * Creates a new vertex attributes object from a existing OpenGL VAO handle
		 */
		OpenGLVertexAttribute(GLuint vao) : vao(vao) { }

		/**
		 * Destroys the Vertex Attribute object
		 */
		~OpenGLVertexAttribute() {
			if(vao != 0) {
				glDeleteVertexArrays(1, &vao);
			}
		}

	public:
		/**
		 * Activates the OpenGL vertex attribute
		 */
		void activate() {
			glBindVertexArray(vao);
		}

		/**
		 * Deacivates the OpenGL vertex attribute
		 */
		void deactivate() {
			glBindVertexArray(0);
		}

	};

}


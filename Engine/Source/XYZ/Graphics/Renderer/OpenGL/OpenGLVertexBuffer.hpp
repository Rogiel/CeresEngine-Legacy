//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/VertexBuffer.hpp"
#include <GL/glew.h>

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		GLuint ebo;

		/**
		 * The OpenGL vertex buffer handle
		 */
		GLuint vertexBuffer = 0;

		/**
		 * The Vertex Attribute Array handle
		 */
		GLuint vao;

		/**
		 * The number of vertex in the buffer
		 */
		GLsizei vertexCount;

	public:
		/**
		 * Creates a new OpenGL compiled mesh object
		 *
		 * @param vertexBuffer the OpenGL vertex buffer handle
		 * @param vao the Vertex Attribute Array handle
		 * @param vertexCount the number of vertex in the buffer
		 */
		OpenGLVertexBuffer(GLuint ebo, GLuint vertexBuffer, GLuint vao,
						   GLsizei vertexCount);

		/**
		 * Relases the OpenGL mesh buffers
		 */
		~OpenGLVertexBuffer();

	public:
		/**
		 * Draws a mesh
		 */
		void draw() final override;

	};

}



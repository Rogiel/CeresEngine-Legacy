//
// Created by Rogiel Sulzbach on 7/8/17.
//

#include "OpenGLVertexBuffer.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	OpenGLVertexBuffer::OpenGLVertexBuffer(GLuint ebo, GLuint vertexBuffer, GLuint vao, GLsizei vertexCount) :
			ebo(ebo),
			vertexBuffer(vertexBuffer),
			vao(vao),
			vertexCount(vertexCount) {}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		if(vertexBuffer != 0) {
			glDeleteBuffers(1, &vertexBuffer);
		}
		if(ebo != 0) {
			glDeleteBuffers(1, &ebo);
		}
		if(vao != 0) {
			glDeleteVertexArrays(1, &vao);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLVertexBuffer::draw() {
		// draw mesh
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

}
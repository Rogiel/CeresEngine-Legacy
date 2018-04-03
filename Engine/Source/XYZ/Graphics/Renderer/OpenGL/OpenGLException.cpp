//
// Created by Rogiel Sulzbach on 7/30/17.
//

#include "OpenGLException.hpp"
#include <GL/glew.h>

namespace XYZ::Graphics::Renderer::OpenGL {

	void throwOpenGLException() {
		GLenum errorCode = glGetError();
		if(errorCode == GL_NO_ERROR) {
			return;
		}

		switch(errorCode) {
			case GL_INVALID_ENUM:
				throw OpenGLInvalidEnumException();

			case GL_INVALID_VALUE:
				throw OpenGLInvalidValueException();

			case GL_INVALID_OPERATION:
				throw OpenGLInvalidOperationException();

			case GL_STACK_OVERFLOW:
				throw OpenGLStackOverflowException();

			case GL_STACK_UNDERFLOW:
				throw OpenGLStackUnderflowException();

			case GL_OUT_OF_MEMORY:
				throw OpenGLOutOfMemoryException();

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				throw OpenGLInvalidFramebufferOperationException();

			default:
				throw OpenGLException();
		}
	}

}
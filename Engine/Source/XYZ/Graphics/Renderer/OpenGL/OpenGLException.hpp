//
// Created by Rogiel Sulzbach on 7/30/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/RendererException.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	XYZ_DECL_EXCEPTION(OpenGLException, RendererException);

	XYZ_DECL_EXCEPTION(OpenGLInvalidEnumException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLInvalidValueException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLInvalidOperationException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLStackOverflowException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLStackUnderflowException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLOutOfMemoryException, OpenGLException);

	XYZ_DECL_EXCEPTION(OpenGLInvalidFramebufferOperationException, OpenGLException);

#ifdef NDEBUG
#define XYZ_OPENGL_CHECK_THROW_DEBUG() (void)0
#else
#define XYZ_OPENGL_CHECK_THROW_DEBUG() throwOpenGLException()
#endif

	void throwOpenGLException();

	template<typename Call, typename... Args>
	auto wrap(Call call, Args... args) {
		struct Checker {
			~Checker() { XYZ_OPENGL_CHECK_THROW_DEBUG(); }
		} checker;
		return call(args...);
	}

}


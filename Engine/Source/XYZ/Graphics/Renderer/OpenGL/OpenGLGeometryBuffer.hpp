//
// Created by Rogiel Sulzbach on 7/21/17.
//

#pragma once

#include "OpenGLFramebuffer.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLGeometryBuffer {
	public:
		/**
		 * The geometry framebuffer
		 */
		OpenGLFramebuffer framebuffer;

		/**
		 * The fragment position and depth texture
		 */
		OpenGLTexture positionDepth;

		/**
		 * The fragment interpolated normal and shininess texture
		 */
		OpenGLTexture normalShininess;

		/**
		 * The fragment diffuse and specular components texture
		 */
		OpenGLTexture albedoSpecular;

		/**
		 * The framebuffer depth buffer
		 */
		unsigned int rboDepth;

		unsigned int width;
		unsigned int height;

	public:
		OpenGLGeometryBuffer(unsigned int width, unsigned int height);
		~OpenGLGeometryBuffer();

	public:
		void activate();
		void deactivate();

	};

}




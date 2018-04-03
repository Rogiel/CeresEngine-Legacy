//
// Created by Rogiel Sulzbach on 7/21/17.
//

#include "OpenGLGeometryBuffer.hpp"
#include "OpenGLException.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	OpenGLGeometryBuffer::OpenGLGeometryBuffer(unsigned int width, unsigned int height) :
			framebuffer(width, height),
			positionDepth(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT),
			normalShininess(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT),
			albedoSpecular(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT),
			width(width), height(height) {
		framebuffer.activate();

		// position + depth color buffer
		positionDepth.setMagnificationMinificationFilter(Texture::TextureMagnification::NEAREST,
														 Texture::TextureMinification::NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
							   positionDepth.textureID, 0);

		// normal + shininess color buffer
		normalShininess.setMagnificationMinificationFilter(Texture::TextureMagnification::NEAREST,
														   Texture::TextureMinification::NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
							   normalShininess.textureID, 0);

		// color + specular color buffer
		albedoSpecular.setMagnificationMinificationFilter(Texture::TextureMagnification::NEAREST,
														  Texture::TextureMinification::NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
							   albedoSpecular.textureID, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);

		// create and attach depth buffer (renderbuffer)
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
							   rboDepth);

		// finally check if framebuffer is complete
		framebuffer.deactivate();
	}

	OpenGLGeometryBuffer::~OpenGLGeometryBuffer() {
		glDeleteRenderbuffers(1, &rboDepth);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLGeometryBuffer::resize(unsigned int width, unsigned height) {
		framebuffer.resize(width, height);
		positionDepth.resize(width, height);
		normalShininess.resize(width, height);
		albedoSpecular.resize(width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		OpenGLGeometryBuffer::width = width;
		OpenGLGeometryBuffer::height = height;
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLGeometryBuffer::activate() {
		framebuffer.activate();
	}

	void OpenGLGeometryBuffer::deactivate() {
		framebuffer.deactivate();
	}

}
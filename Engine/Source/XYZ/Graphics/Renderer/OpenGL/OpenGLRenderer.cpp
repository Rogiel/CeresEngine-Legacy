//
// Created by Rogiel Sulzbach on 7/2/17.
//

#include "OpenGLRenderer.hpp"

#include <GL/glew.h>

#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLFramebuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	OpenGLRenderer::OpenGLRenderer() : defaultFramebuffer(0, 1024, 768) {
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_MULTISAMPLE);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Cull triangles which normal is not towards the camera
//		glEnable(GL_CULL_FACE);
	}

	OpenGLRenderer::~OpenGLRenderer() {}

	// -----------------------------------------------------------------------------------------------------------------

	MeshCompiler& OpenGLRenderer::getMeshCompiler() {
		return compiler;
	}

	ShaderCompiler& OpenGLRenderer::getShaderCompiler() {
		return compiler;
	}

	TextureCompiler& OpenGLRenderer::getTextureCompiler() {
		return compiler;
	}

	Material::MaterialFactory& OpenGLRenderer::getMaterialFactory() {
		return compiler;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Framebuffer& OpenGLRenderer::getDefaultFramebuffer() {
		return defaultFramebuffer;
	}

	// -----------------------------------------------------------------------------------------------------------------

	static std::unique_ptr<Framebuffer> _createFramebuffer(OpenGLRenderer& renderer, size_t width, size_t height, GLenum textureKind = GL_RGB, GLenum fragmentType = GL_UNSIGNED_BYTE, GLenum attachment = GL_COLOR_ATTACHMENT0) {
		// create the framebuffer object
		GLuint framebufferID;
		glGenFramebuffers(1, &framebufferID);

		// create depth texture
		GLuint framebufferTextureID;
		glGenTextures(1, &framebufferTextureID);
		glBindTexture(GL_TEXTURE_2D, framebufferTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, textureKind, static_cast<GLuint>(width), static_cast<GLuint>(height), 0,
					 textureKind, fragmentType, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, framebufferTextureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return std::make_unique<OpenGLFramebuffer>(
				framebufferID, static_cast<unsigned int>(width), static_cast<unsigned int>(height),  std::make_shared<OpenGLTexture>(framebufferTextureID)
		);
	}

	Framebuffer::Ptr OpenGLRenderer::createFramebuffer(size_t width, size_t height, bool withColor, bool withDepth) {
		return nullptr;
	}

	Framebuffer::Ptr OpenGLRenderer::createColorFramebuffer(size_t width, size_t height) {
		return _createFramebuffer(*this, width, height, GL_RGB, GL_UNSIGNED_BYTE);
	}

	Framebuffer::Ptr OpenGLRenderer::createDepthFramebuffer(size_t width, size_t height) {
		return _createFramebuffer(*this, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	}

}
//
// Created by Rogiel Sulzbach on 7/8/17.
//

#include "OpenGLFramebuffer.hpp"

#include "OpenGLRenderer.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLVertexBuffer.hpp"
#include "OpenGLShaderBuffers.hpp"

#include "XYZ/Scene/Light/DirectionalLight.hpp"
#include "XYZ/Scene/Light/PointLight.hpp"
#include "XYZ/Scene/Light/SpotLight.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	// -----------------------------------------------------------------------------------------------------------------

	OpenGLFramebuffer::OpenGLFramebuffer() :
			width(0), height(0) {
		glGenFramebuffers(1, &framebufferID);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(unsigned int width, unsigned int height) :
			width(width), height(height) {
		glGenFramebuffers(1, &framebufferID);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(GLuint framebufferID, unsigned int width, unsigned int height,
										 std::shared_ptr<OpenGLTexture> texture) :
			framebufferID(framebufferID), texture(texture),
			width(width), height(height) {}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		if(framebufferID != 0) {
			glDeleteFramebuffers(1, &framebufferID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	static void enableOrDisable(GLenum name, bool state) {
		(state ? glEnable : glDisable)(name);
	}

	static void updateFramebufferState(OpenGLFramebuffer::Configuration& configuration) {
		enableOrDisable(GL_CULL_FACE, configuration.faceCulling);
		enableOrDisable(GL_DEPTH_TEST, configuration.depthTesting);
		enableOrDisable(GL_BLEND, configuration.blending);
	}

	void OpenGLFramebuffer::activate() {
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		updateFramebufferState(configuration);
	}

	void OpenGLFramebuffer::deactivate() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLFramebuffer::resize(unsigned int width, unsigned int height) {
		OpenGLFramebuffer::width = width;
		OpenGLFramebuffer::height = height;
	}

	void OpenGLFramebuffer::copy(Framebuffer& destinationFramebuffer) const {
		auto& destination = static_cast<OpenGLFramebuffer&>(destinationFramebuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.framebufferID);
		glBlitFramebuffer(0, 0, width, height, 0, 0, destination.width,
						  destination.height, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::clear(glm::vec4 color) {
		// Clear the framebuffer contents
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// -----------------------------------------------------------------------------------------------------------------

	OpenGLFramebuffer& OpenGLFramebuffer::blending(bool state) {
		configuration.blending = true;
		enableOrDisable(GL_BLEND, configuration.blending);
		return *this;
	}

	bool OpenGLFramebuffer::blending() const {
		return configuration.blending;
	}

	OpenGLFramebuffer& OpenGLFramebuffer::depthTest(bool state) {
		configuration.depthTesting = state;
		enableOrDisable(GL_DEPTH_TEST, configuration.depthTesting);
		return *this;
	}

	bool OpenGLFramebuffer::depthTest() const {
		return configuration.depthTesting;
	}

	OpenGLFramebuffer& OpenGLFramebuffer::faceCulling(bool state) {
		configuration.faceCulling = state;
		enableOrDisable(GL_CULL_FACE, configuration.faceCulling);
		return *this;
	}

	bool OpenGLFramebuffer::faceCulling() const {
		return configuration.faceCulling;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Texture::Texture::Ptr OpenGLFramebuffer::getTexture() {
		return texture;
	}

}
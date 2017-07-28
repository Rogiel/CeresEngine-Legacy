//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/Framebuffer.hpp"

#include "XYZ/Graphics/Renderer/OpenGL/OpenGLTexture.hpp"
#include "XYZ/Graphics/Renderer/OpenGL/OpenGLShaderBuffers.hpp"
#include "XYZ/Graphics/Renderer/OpenGL/OpenGLBuffer.hpp"

#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLFramebuffer : public Framebuffer {
		friend class OpenGLRenderer;

	public:
		/**
		 * The OpenGL Framebuffer ID
		 */
		GLuint framebufferID;

		/**
		 * The texture this framebuffer is mapped to
		 */
		std::shared_ptr<OpenGLTexture> texture;

	public:
		struct Configuration {
			bool faceCulling = true;
			bool depthTesting = true;
			bool blending = false;
		};

	private:
		Configuration configuration;

	private:
		/**
		 * The framebuffer width
		 */
		unsigned int width;

		/**
		 * The framebuffer height
		 */
		unsigned int height;

	public:
		/**
		 * Create a empty OpenGL framebuffer
		 */
		OpenGLFramebuffer();

		/**
		 * Creates a new OpenGL framebuffer
		 *
		 * @param width the framebuffer width
		 * @param height the framebuffer height
		 */
		OpenGLFramebuffer(unsigned int width, unsigned int height);

		/**
		 * Create a new OpenGL framebuffer object
		 *
		 * @param framebufferID the framebuffer object ID
		 * @param width the framebuffer width
		 * @param height the framebuffer height
		 * @param texture the texture this framebuffer is mapped to
		 */
		OpenGLFramebuffer(GLuint framebufferID, unsigned int width, unsigned int height,
						  std::shared_ptr<OpenGLTexture> texture = nullptr);

		/**
		 * Releases the OpenGL framebuffer
		 */
		virtual ~OpenGLFramebuffer();

	public:
		/**
		 * Activates the framebuffer
		 */
		void activate() override;

		/**
		 * Deactivates the framebuffer
		 */
		void deactivate() override;

	public:
		void resize(unsigned int width, unsigned int height);

		/**
		 * Copy the contents of the framebuffer to the <tt>destination</tt>
		 * framebuffer.
		 *
		 * @param destination the destination framebuffer
		 */
		void copy(Framebuffer& destination) const override;

		/**
		 * Clear the framebuffer contents
		 *
		 * @param color the color to set the buffer to
		 */
		void clear(glm::vec4 color = glm::vec4(0.0)) override;

	public:
		/**
		 * Enables or disables the blending mode on the framebuffer
		 *
		 * @param state true to enable blending, false to disable.
		 */
		OpenGLFramebuffer& blending(bool state) final;

		/**
		 * Checks the blending mode state
		 *
		 * @return true if blending is enabled, false if disabled.
		 */
		bool blending() const final;

		/**
		 * Enables or disables the depth testing on the framebuffer
		 *
		 * @param state true to enable depth testing, false to disable.
		 */
		OpenGLFramebuffer& depthTest(bool state) final;

		/**
		 * Checks the depth testing state
		 *
		 * @return true if depth testing is enabled, false if disabled.
		 */
		bool depthTest() const final;

		/**
		 * Enables or disables the face culling on the framebuffer
		 *
		 * @param state true to enable face culling, false to disable.
		 */
		OpenGLFramebuffer& faceCulling(bool state) final;

		/**
		 * Checks the face culling  state
		 *
		 * @return true if face culling is enabled, false if disabled.
		 */
		bool faceCulling() const final;

	public:
		/**
		 * Creates a new texture from the framebuffer contents.
		 *
		 * NOTE: implementations are allowed to return a reusable texture.
		 *
		 * @return a texture with the framebuffer contents
		 */
		Texture::Texture::Ptr getTexture() final override;

	};

}



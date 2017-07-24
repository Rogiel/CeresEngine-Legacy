//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Graphics/Texture/Texture.hpp"
#include "XYZ/Scene/Scene.hpp"

#include <memory>

namespace XYZ::Graphics::Renderer {

	class Framebuffer {
	public:
		/**
		 * A pointer to a framebuffer
		 */
		using Ptr = std::unique_ptr<Framebuffer>;

	public:
		/**
		 * Activates the framebuffer
		 */
		virtual void activate() = 0;

		/**
		 * Deactivates the framebuffer
		 */
		virtual void deactivate() = 0;

	public:
		/**
		 * Copy the contents of the framebuffer to the <tt>destination</tt>
		 * framebuffer.
		 *
		 * @param destination the destination framebuffer
		 */
		virtual void copy(Framebuffer& destination) const = 0;

		/**
		 * Clear the framebuffer contents
		 *
		 * @param color the color to set the buffer to
		 */
		virtual void clear(glm::vec4 color = glm::vec4(0.0)) = 0;

	public:
		/**
		 * Enables or disables the blending mode on the framebuffer
		 *
		 * @param state true to enable blending, false to disable.
		 *
		 * @return this
		 */
		virtual Framebuffer& blending(bool state) = 0;

		/**
		 * Checks the blending mode state
		 *
		 * @return true if blending is enabled, false if disabled.
		 */
		virtual bool blending() const = 0;

		/**
		 * Enables or disables the depth testing on the framebuffer
		 *
		 * @param state true to enable depth testing, false to disable.
		 *
		 * @return this
		 */
		virtual Framebuffer& depthTest(bool state) = 0;

		/**
		 * Checks the depth testing state
		 *
		 * @return true if depth testing is enabled, false if disabled.
		 */
		virtual bool depthTest() const = 0;

		/**
		 * Enables or disables the face culling on the framebuffer
		 *
		 * @param state true to enable face culling, false to disable.
		 *
		 * @return this
		 */
		virtual Framebuffer& faceCulling(bool state) = 0;

		/**
		 * Checks the face culling  state
		 *
		 * @return true if face culling is enabled, false if disabled.
		 */
		virtual bool faceCulling() const = 0;

	public:
		/**
		 * Creates a new texture from the framebuffer contents.
		 *
		 * NOTE: implementations are allowed to return a reusable texture.
		 *
		 * @return a texture with the framebuffer contents
		 */
		virtual Texture::Texture::Ptr getTexture() = 0;

	};

}



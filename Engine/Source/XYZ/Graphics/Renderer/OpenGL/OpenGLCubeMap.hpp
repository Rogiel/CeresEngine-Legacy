//
// Created by Rogiel Sulzbach on 7/27/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/OpenGL/OpenGLTexture.hpp"

#include <cstdint>
#include <GL/glew.h>

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLCubeMap {
	public:
		GLuint textureID;
		GLint internalFormat;
		GLenum format;
		GLenum type;

	public:
		/**
		 * Creates a new empty texture
		 */
		OpenGLCubeMap();

		/**
		 * Create a new OpenGL texture with the given parameter.
		 *
		 * @param width the texture width
		 * @param height the texture height
		 * @param internalFormat the texture internal format
		 * @param format the texture content format
		 * @param type the texture data type
		 */
		OpenGLCubeMap(size_t width, size_t height, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA,
				GLenum type = GL_UNSIGNED_BYTE);

		/**
		 * Creates a new OpenGL texture from a already existing texture handle
		 *
		 * @param textureID the OpenGL texture handle
		 */
		explicit OpenGLCubeMap(GLuint textureID);

		/**
		 * Destroys the texture.
		 */
		~OpenGLCubeMap();

	public:
		/**
		 * Activate the texture
		 *
		 * @param slot the texture slot
		 */
		void activate(unsigned int slot = 0);

		/**
		 * Deactivate the texture
		 *
		 * @param slot the texture slot
		 */
		void deactivate(unsigned int slot);

	public:
		/**
		 * Checks if the texture can be resized
		 *
		 * @return true if the texture can be resized, false otherwise
		 */
		bool canResize() const;

		/**
		 * Resizes the texture.
		 *
		 * @param width the new texture width
		 * @param height the new texture height
		 */
		void resize(size_t width, size_t height);

	public:
		/**
		 * Sets the wrap mode for texture coordinate s.
		 *
		 * @param mode the wrap mode for texture coordinate s.
		 */
		void setWrapModeS(Graphics::Texture::TextureWrap mode);

		/**
		 * Gets the wrap mode for texture coordinate s.
		 *
		 * @return the wrap mode for texture coordinate s.
		 */
		Graphics::Texture::TextureWrap getWrapModeS() const;

		/**
		 * Sets the wrap mode for texture coordinate t.
		 *
		 * @param mode the wrap mode for texture coordinate t.
		 */
		void setWrapModeT(Graphics::Texture::TextureWrap mode);

		/**
		 * Gets the wrap mode for texture coordinate t.
		 *
		 * @return the wrap mode for texture coordinate t.
		 */
		Graphics::Texture::TextureWrap getWrapModeT() const;

		/**
		 * Sets the wrap mode for texture coordinate r.
		 *
		 * @param mode the wrap mode for texture coordinate r.
		 */
		void setWrapModeR(Graphics::Texture::TextureWrap mode);

		/**
		 * Gets the wrap mode for texture coordinate r.
		 *
		 * @return the wrap mode for texture coordinate r.
		 */
		Graphics::Texture::TextureWrap getWrapModeR() const;

		/**
		 * Sets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @param borderColor the color to be used when wrapping
		 */
		void setBorderColor(glm::vec4 borderColor);

		/**
		 * Gets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @return the color to be used when wrapping
		 */
		glm::vec4 getBorderColor() const;

	public:
		/**
		 * Sets the texture magnification function is used whenever the
		 * level-of-detail function used when sampling from the texture
		 * determines that the texture should be magified.
		 *
		 * @param filter the filter to be used when magnifying
		 */
		void setMagnificationFilter(Graphics::Texture::TextureMagnification filter);

		/**
		 * Gets the texture magnification function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be magified.
		 *
		 * @return the filter to be used when magifying
		 */
		Graphics::Texture::TextureMagnification getMagnificationFilter() const;

		/**
		 * Sets the texture minifying function is used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @param filter the filter to be used when minifying
		 */
		void setMinificationFilter(Graphics::Texture::TextureMinification filter);

		/**
		 * Gets the texture minifying function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @return the filter to be used when minifying
		 */
		Graphics::Texture::TextureMinification getMinificationFilter() const;

	};

}


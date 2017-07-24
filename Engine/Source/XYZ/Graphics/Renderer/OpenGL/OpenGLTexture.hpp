//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include <GL/glew.h>
#include "XYZ/Graphics/Texture/Texture.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLTexture : public Texture::Texture {
	public:
		GLuint textureID;
		GLint internalFormat;
		GLenum format;
		GLenum type;

	public:
		/**
		 * Creates a new empty texture
		 */
		OpenGLTexture();

		/**
		 * Creates a new OpenGL texture from a texture image
		 *
		 * @param textureImage the texture image to create a new OpenGL
		 * texture with
		 */
		explicit OpenGLTexture(const Graphics::Texture::TextureImage& textureImage);

		/**
		 * Create a new OpenGL texture with the given parameter.
		 *
		 * @param width the texture width
		 * @param height the texture height
		 * @param internalFormat the texture internal format
		 * @param format the texture content format
		 * @param type the texture data type
		 */
		OpenGLTexture(size_t width, size_t height, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA,
					  GLenum type = GL_UNSIGNED_BYTE);

		/**
		 * Creates a new OpenGL texture from a already existing texture handle
		 *
		 * @param textureID the OpenGL texture handle
		 */
		explicit OpenGLTexture(GLuint textureID);

		/**
		 * Destroys the texture.
		 */
		~OpenGLTexture();

	public:
		/**
		 * Activate the texture
		 *
		 * @param slot the texture slot
		 */
		void activate(unsigned int slot = 0) final;

		/**
		 * Deactivate the texture
		 *
		 * @param slot the texture slot
		 */
		void deactivate(unsigned int slot) final;

	public:
		/**
		 * @return the number of channels in each texel
		 */
		int getNumberOfChannels() const final;

		/**
		 * @return the number of bytes in each texel channel
		 */
		int getBytesPerChannel() const final;

	public:
		/**
		 * Checks if the texture can be resized
		 *
		 * @return true if the texture can be resized, false otherwise
		 */
		bool canResize() const final;

		/**
		 * Resizes the texture.
		 *
		 * @param width the new texture width
		 * @param height the new texture height
		 */
		void resize(size_t width, size_t height) final;

		/**
		 * Checks if the texture image can be update
		 *
		 * @return true if the texture can be updated, false otherwise
		 */
		bool canUpdate() final;

		/**
		 * Updates the texture image data.
		 *
		 * @param textureImage the new texture to update with
		 */
		void update(const Graphics::Texture::TextureImage& textureImage) final;

	public:
		/**
		 * Sets the wrap mode for texture coordinate s.
		 *
		 * @param mode the wrap mode for texture coordinate s.
		 */
		void setWrapModeS(Graphics::Texture::TextureWrap mode) final;

		/**
		 * Gets the wrap mode for texture coordinate s.
		 *
		 * @return the wrap mode for texture coordinate s.
		 */
		Graphics::Texture::TextureWrap getWrapModeS() const final;

		/**
		 * Sets the wrap mode for texture coordinate t.
		 *
		 * @param mode the wrap mode for texture coordinate t.
		 */
		void setWrapModeT(Graphics::Texture::TextureWrap mode) final;

		/**
		 * Gets the wrap mode for texture coordinate t.
		 *
		 * @return the wrap mode for texture coordinate t.
		 */
		Graphics::Texture::TextureWrap getWrapModeT() const final;

		/**
		 * Sets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @param borderColor the color to be used when wrapping
		 */
		void setBorderColor(glm::vec4 borderColor) final;

		/**
		 * Gets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @return the color to be used when wrapping
		 */
		glm::vec4 getBorderColor() const final;

	public:
		/**
		 * Sets the texture magnification function is used whenever the
		 * level-of-detail function used when sampling from the texture
		 * determines that the texture should be magified.
		 *
		 * @param filter the filter to be used when magnifying
		 */
		void setMagnificationFilter(Graphics::Texture::TextureMagnification filter) final;

		/**
		 * Gets the texture magnification function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be magified.
		 *
		 * @return the filter to be used when magifying
		 */
		Graphics::Texture::TextureMagnification getMagnificationFilter() const final;

		/**
		 * Sets the texture minifying function is used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @param filter the filter to be used when minifying
		 */
		void setMinificationFilter(Graphics::Texture::TextureMinification filter) final;

		/**
		 * Gets the texture minifying function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @return the filter to be used when minifying
		 */
		Graphics::Texture::TextureMinification getMinificationFilter() const final;

		/**
		 * Checks if the texture object supports mipmap generation
		 *
		 * @return true if mipmap generation is supported
		 */
		bool canGenerateMipmaps() const final;

		/**
		 * Generate mipmaps for the texture.
		 */
		void generateMipmaps() final;

	public:
		/**
		 * Creates a new texture image from the contents of the texture
		 *
		 * @return a newly created texture image with the contents of this texture
		 */
		Graphics::Texture::TextureImage::Ptr toImage() const final;

	};

}


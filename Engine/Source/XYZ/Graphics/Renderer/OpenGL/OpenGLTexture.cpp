//
// Created by Rogiel Sulzbach on 7/8/17.
//

#include "OpenGLTexture.hpp"

#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	template<typename Executor>
	auto wrap(GLuint textureID, Executor&& executor) {
		glBindTexture(GL_TEXTURE_2D, textureID);

		struct Cleanup {
			~Cleanup() {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		} cleanup;
		return executor();
	}

	// -----------------------------------------------------------------------------------------------------------------

	OpenGLTexture::OpenGLTexture() :
			textureID(0) {}

	OpenGLTexture::OpenGLTexture(const Graphics::Texture::TextureImage& textureImage) {
		glGenTextures(1, &textureID);

		internalFormat = GL_RGB8;
		format = (textureImage.isAlpha() ? GL_RGBA : GL_RGB);
		type = GL_UNSIGNED_BYTE;

		wrap(textureID, [=, &textureImage]() {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureImage.getWidth(), textureImage.getHeight(),
						 0, format, type, textureImage.getRaw().data());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		});
	}

	OpenGLTexture::OpenGLTexture(size_t width, size_t height, GLint internalFormat, GLenum format, GLenum type) :
			internalFormat(internalFormat), format(format), type(type) {
		glGenTextures(1, &textureID);
		wrap(textureID, [=]() {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLuint) width, (GLuint) height, 0, format, type, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		});
	}

	OpenGLTexture::OpenGLTexture(GLuint textureID) :
			textureID(textureID) {}

	OpenGLTexture::~OpenGLTexture() {
		if(textureID != 0) {
			glDeleteTextures(1, &textureID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLTexture::activate(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void OpenGLTexture::deactivate(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	int OpenGLTexture::getNumberOfChannels() const {
		return 0; // TODO
	}

	int OpenGLTexture::getBytesPerChannel() const {
		return 0; // TODO
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool OpenGLTexture::canResize() const {
		return true;
	}

	void OpenGLTexture::resize(size_t width, size_t height) {
		wrap(textureID, [=]() {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLuint) width, (GLuint) height, 0, format, type, nullptr);
		});
	}

	bool OpenGLTexture::canUpdate() {
		return true;
	}

	void OpenGLTexture::update(const Graphics::Texture::TextureImage& textureImage) {
		wrap(textureID, [=, &textureImage]() {
			internalFormat = GL_RGB8;
			format = (textureImage.isAlpha() ? GL_RGBA : GL_RGB);
			type = GL_UNSIGNED_BYTE;

			glTexSubImage2D(GL_TEXTURE_2D, 0, internalFormat, textureImage.getWidth(), textureImage.getHeight(),
						 0, format, type, textureImage.getRaw().data());
		});
	}

	// -----------------------------------------------------------------------------------------------------------------

	static struct {
		Texture::TextureWrap wrap;
		GLenum gl;
	} TextureWrapMapping[] = {
			{Texture::TextureWrap::CLAMP_TO_EDGE,        GL_CLAMP_TO_EDGE},
			{Texture::TextureWrap::CLAMP_TO_BORDER,      GL_CLAMP_TO_BORDER},
			{Texture::TextureWrap::REPEAT,               GL_REPEAT},
			{Texture::TextureWrap::MIRRORED_REPEAT,      GL_MIRRORED_REPEAT},
			{Texture::TextureWrap::MIRROR_CLAMP_TO_EDGE, GL_MIRROR_CLAMP_TO_EDGE}
	};

	void OpenGLTexture::setWrapModeS(Graphics::Texture::TextureWrap mode) {
		for(auto& map : TextureWrapMapping) {
			if(map.wrap == mode) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureWrap OpenGLTexture::getWrapModeS() const {
		GLenum mode;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint*) &mode);
		});

		for(auto& map : TextureWrapMapping) {
			if(map.gl == mode) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureWrap(0);
	}

	void OpenGLTexture::setWrapModeT(Graphics::Texture::TextureWrap mode) {
		for(auto& map : TextureWrapMapping) {
			if(map.wrap == mode) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureWrap OpenGLTexture::getWrapModeT() const {
		GLenum mode;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint*) &mode);
		});

		for(auto& map : TextureWrapMapping) {
			if(map.gl == mode) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureWrap(0);
	}

	void OpenGLTexture::setBorderColor(glm::vec4 borderColor) {
		wrap(textureID, [=]() {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
		});
	}

	glm::vec4 OpenGLTexture::getBorderColor() const {
		glm::vec4 borderColor;

		wrap(textureID, [&]() {
			glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
		});

		return borderColor;
	}

	// -----------------------------------------------------------------------------------------------------------------

	static constexpr struct {
		Texture::TextureMagnification wrap;
		GLenum gl;
	} TextureMagnitificationMapping[] = {
			{Texture::TextureMagnification::NEAREST, GL_NEAREST},
			{Texture::TextureMagnification::LINEAR,  GL_LINEAR}
	};

	void OpenGLTexture::setMagnificationFilter(Graphics::Texture::TextureMagnification filter) {
		for(auto& map : TextureMagnitificationMapping) {
			if(map.wrap == filter) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureMagnification OpenGLTexture::getMagnificationFilter() const {
		GLenum filter;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint*) &filter);
		});

		for(auto& map : TextureMagnitificationMapping) {
			if(map.gl == filter) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureMagnification(0);
	}

	static constexpr struct {
		Texture::TextureMinification wrap;
		GLenum gl;
	} TextureMinificationMapping[] = {
			{Texture::TextureMinification::NEAREST,                GL_NEAREST},
			{Texture::TextureMinification::LINEAR,                 GL_LINEAR},
			{Texture::TextureMinification::NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST},
			{Texture::TextureMinification::LINEAR_MIPMAP_NEAREST,  GL_LINEAR_MIPMAP_NEAREST},
			{Texture::TextureMinification::NEAREST_MIPMAP_LINEAR,  GL_NEAREST_MIPMAP_LINEAR},
			{Texture::TextureMinification::LINEAR_MIPMAP_LINEAR,   GL_LINEAR_MIPMAP_LINEAR}
	};

	void OpenGLTexture::setMinificationFilter(Graphics::Texture::TextureMinification filter) {
		for(auto& map : TextureMinificationMapping) {
			if(map.wrap == filter) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureMinification OpenGLTexture::getMinificationFilter() const {
		GLenum filter;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint*) &filter);
		});

		for(auto& map : TextureMinificationMapping) {
			if(map.gl == filter) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureMinification(0);
	}

	bool OpenGLTexture::canGenerateMipmaps() const {
		return true;
	}

	void OpenGLTexture::generateMipmaps() {
		wrap(textureID, []() {
			glGenerateMipmap(GL_TEXTURE_2D);
		});
	}

	// -----------------------------------------------------------------------------------------------------------------

	Texture::TextureImage::Ptr OpenGLTexture::toImage() const {
		return nullptr; // TODO
	}

}
//
// Created by Rogiel Sulzbach on 7/27/17.
//

#include "OpenGLCubeMap.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {


	template<typename Executor>
	auto wrap(GLuint textureID, Executor&& executor) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		struct Cleanup {
			~Cleanup() {
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
		} cleanup;
		return executor();
	}

	// -----------------------------------------------------------------------------------------------------------------

	OpenGLCubeMap::OpenGLCubeMap() :
			textureID(0) {}

	OpenGLCubeMap::OpenGLCubeMap(size_t width, size_t height, GLint internalFormat, GLenum format, GLenum type) :
			internalFormat(internalFormat), format(format), type(type) {
		glGenTextures(1, &textureID);
		wrap(textureID, [=]() {
			for(unsigned int i = 0; i < 6; ++i) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, (GLuint) width, (GLuint) height, 0,
							 format, type, nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		});
	}

	OpenGLCubeMap::OpenGLCubeMap(GLuint textureID) :
			textureID(textureID) {}

	OpenGLCubeMap::~OpenGLCubeMap() {
		if(textureID != 0) {
			glDeleteTextures(1, &textureID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLCubeMap::activate(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}

	void OpenGLCubeMap::deactivate(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool OpenGLCubeMap::canResize() const {
		return true;
	}

	void OpenGLCubeMap::resize(size_t width, size_t height) {
		wrap(textureID, [=]() {
			for(unsigned int i = 0; i < 6; ++i) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, (GLuint) width, (GLuint) height, 0,
							 format, type, nullptr);
			}
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

	void OpenGLCubeMap::setWrapModeS(Graphics::Texture::TextureWrap mode) {
		for(auto& map : TextureWrapMapping) {
			if(map.wrap == mode) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureWrap OpenGLCubeMap::getWrapModeS() const {
		GLenum mode;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint*) &mode);
		});

		for(auto& map : TextureWrapMapping) {
			if(map.gl == mode) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureWrap(0);
	}

	void OpenGLCubeMap::setWrapModeT(Graphics::Texture::TextureWrap mode) {
		for(auto& map : TextureWrapMapping) {
			if(map.wrap == mode) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureWrap OpenGLCubeMap::getWrapModeT() const {
		GLenum mode;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint*) &mode);
		});

		for(auto& map : TextureWrapMapping) {
			if(map.gl == mode) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureWrap(0);
	}

	void OpenGLCubeMap::setWrapModeR(Graphics::Texture::TextureWrap mode) {
		for(auto& map : TextureWrapMapping) {
			if(map.wrap == mode) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureWrap OpenGLCubeMap::getWrapModeR() const {
		GLenum mode;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLint*) &mode);
		});

		for(auto& map : TextureWrapMapping) {
			if(map.gl == mode) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureWrap(0);
	}

	void OpenGLCubeMap::setBorderColor(glm::vec4 borderColor) {
		wrap(textureID, [=]() {
			glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
		});
	}

	glm::vec4 OpenGLCubeMap::getBorderColor() const {
		glm::vec4 borderColor;

		wrap(textureID, [&]() {
			glGetTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
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

	void OpenGLCubeMap::setMagnificationFilter(Graphics::Texture::TextureMagnification filter) {
		for(auto& map : TextureMagnitificationMapping) {
			if(map.wrap == filter) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureMagnification OpenGLCubeMap::getMagnificationFilter() const {
		GLenum filter;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint*) &filter);
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

	void OpenGLCubeMap::setMinificationFilter(Graphics::Texture::TextureMinification filter) {
		for(auto& map : TextureMinificationMapping) {
			if(map.wrap == filter) {
				return wrap(textureID, [=]() {
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, map.gl);
				});
			}
		}
	}

	Graphics::Texture::TextureMinification OpenGLCubeMap::getMinificationFilter() const {
		GLenum filter;

		wrap(textureID, [&]() {
			glGetTextureParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint*) &filter);
		});

		for(auto& map : TextureMinificationMapping) {
			if(map.gl == filter) {
				return map.wrap;
			}
		}

		return Graphics::Texture::TextureMinification(0);
	}

}
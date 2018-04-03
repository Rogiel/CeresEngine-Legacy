//
// Created by Rogiel Sulzbach on 7/5/17.
//

#include "TextureImage.hpp"

#include "Texture.hpp"

namespace XYZ::Graphics::Texture {

	TextureImage::TextureImage(uint32_t width, uint32_t height, std::vector<char> raw, bool alpha) :
			width(width), height(height), raw(std::move(raw)), alpha(alpha) {}


	uint32_t TextureImage::getWidth() const {
		return width;
	}

	uint32_t TextureImage::getHeight() const {
		return height;
	}

	const std::vector<char>& TextureImage::getRaw() const {
		return raw;
	}

	bool TextureImage::isAlpha() const {
		return alpha;
	}

	const Texture::Ptr& TextureImage::getCompiledTexture() const {
		return compiledTexture;
	}

	void TextureImage::setCompiledTexture(const Texture::Ptr& compiledTexture) {
		TextureImage::compiledTexture = compiledTexture;
	}

}
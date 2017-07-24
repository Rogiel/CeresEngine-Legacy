//
// Created by Rogiel Sulzbach on 7/5/17.
//

#include "Texture.hpp"

namespace XYZ::Graphics::Texture {

	void Texture::setWrapMode(TextureWrap s, TextureWrap t) {
		setWrapModeS(s);
		setWrapModeT(t);
	}

	std::tuple<TextureWrap, TextureWrap> Texture::getWrapMode() const {
		return {getWrapModeS(), getWrapModeT()};
	}

	void Texture::setMagnificationMinificationFilter(TextureMagnification magnificationFilter,
													 TextureMinification minificationFilter) {
		setMagnificationFilter(magnificationFilter);
		setMinificationFilter(minificationFilter);
	}

	std::tuple<TextureMagnification, TextureMinification> Texture::getMagnificationMinificationFilter() const {
		return {getMagnificationFilter(), getMinificationFilter()};
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool Texture::didReceiveMemoryWarning() {
//		if(textureImage) {
//			textureImage = nullptr;
//		}
		return Resource::didReceiveMemoryWarning();
	}
}
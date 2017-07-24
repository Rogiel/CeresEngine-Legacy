//
// Created by Rogiel Sulzbach on 7/7/17.
//

#pragma once

#include <memory>

#include "XYZ/Graphics/Texture/Texture.hpp"
#include "XYZ/Graphics/Texture/TextureImage.hpp"

namespace XYZ::Graphics::Renderer {

	/**
	 * The texture compiler is responsible for taking a raw image
	 * as given by a TextureImage and compile it into a renderer
	 * specific texture object.
	 *
	 * On most renderers, compiling a Texture means that the texture
	 * will be loaded into the users graphics card.
	 */
	class TextureCompiler {
	public:
		/**
         * Compiles a vertex shader
         *
         * @return the compiled vertex shader
         */
		virtual Texture::Texture::Ptr compileTexture(
				const Texture::TextureImage& textureImage) = 0;
	};

}


//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include "XYZ/Graphics/Texture/TextureImageLoader.hpp"

namespace XYZ::Graphics::Texture::Stbi {

	class StbiTextureImageLoader : public TextureImageLoader {
	public:
		/**
		 * Checks if the given resource input is supported by the loader
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return true if the resource is supported and can be loaded
		 * with this loader
		 */
		bool supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) override;

		/**
		 * Loads a resource.
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return the loaded resource
		 */
		TextureImage::Ptr load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) override;
	};

}




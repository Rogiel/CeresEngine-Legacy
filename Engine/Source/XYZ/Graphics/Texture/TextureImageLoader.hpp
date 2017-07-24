//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include "XYZ/Resource/ResourceLoader.hpp"
#include "TextureImage.hpp"
#include "XYZ/Resource/Locator/ResourceStream.hpp"

#include <memory>

namespace XYZ::Graphics::Texture {

	using TextureImageLoader = Resource::ResourceLoader<TextureImage, std::unique_ptr<Resource::Locator::ResourceStream>>;

}


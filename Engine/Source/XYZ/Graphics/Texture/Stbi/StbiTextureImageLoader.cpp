//
// Created by Rogiel Sulzbach on 7/14/17.
//

#include <memory>
#include "StbiTextureImageLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace XYZ::Graphics::Texture::Stbi {

	bool StbiTextureImageLoader::supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) {
		return true;
	}

	TextureImage::Ptr StbiTextureImageLoader::load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) {
		stbi_io_callbacks callbacks;
		callbacks.read = [](void* user, char* data, int size) -> int {
			auto stream = reinterpret_cast<Resource::Locator::ResourceStream*>(user);
			return static_cast<int>(stream->read((uint8_t*) data, size));
		};
		callbacks.eof = [](void* user) -> int {
			auto stream = reinterpret_cast<Resource::Locator::ResourceStream*>(user);
			return stream->hasData() ? 1 : 0;
		};
		callbacks.skip = [](void* user, int size) {
			auto stream = reinterpret_cast<Resource::Locator::ResourceStream*>(user);
			int done = 0;

			char buffer[1024];
			while(done < size) {
				done += static_cast<int>(stream->read((uint8_t*) buffer, std::min(1024, size - done)));
			}
		};

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		auto data = (char*) stbi_load_from_callbacks(&callbacks, resourceStream.get(), &width, &height, &nrChannels, 0);

		auto image = std::make_shared<TextureImage>(
				width, height,
				std::vector<char>(data, data + (width * height * nrChannels)),
				nrChannels == 4
		);
		stbi_image_free(data);
		return image;
	}

}
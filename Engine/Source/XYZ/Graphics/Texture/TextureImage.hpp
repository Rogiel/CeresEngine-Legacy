//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include <vector>
#include <string>

namespace XYZ::Graphics::Texture {

	class Texture;

    class TextureImage: public Resource::Resource<TextureImage> {
    private:
        uint32_t width;
        uint32_t height;
        std::vector<char> raw;
		bool alpha = false;

		std::shared_ptr<class Texture> compiledTexture;

    public:
        TextureImage(uint32_t width, uint32_t height, std::vector<char> raw, bool alpha);

    public:
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        const std::vector<char>& getRaw() const;

		bool isAlpha() const;

		const std::shared_ptr<Texture>& getCompiledTexture() const;
		void setCompiledTexture(const std::shared_ptr<Texture>& compiledTexture);

	};

}



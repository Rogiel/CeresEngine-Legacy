//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include "OggVorbisStreamLoader.hpp"

namespace XYZ::Audio::Loader::OggVorbis {

	bool OggVorbisStreamLoader::supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) {
		return false;
	}

	AudioStream::Ptr OggVorbisStreamLoader::load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) {
		return nullptr;
	}

}
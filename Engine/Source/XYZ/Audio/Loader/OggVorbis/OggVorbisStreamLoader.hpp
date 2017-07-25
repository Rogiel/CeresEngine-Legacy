//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Audio/Loader/AudioLoader.hpp"

namespace XYZ::Audio::Loader::OggVorbis {

	class OggVorbisStreamLoader : public AudioStreamLoader {
	public:
		/**
		 * Checks if the given resource input is supported by the loader
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return true if the resource is supported and can be loaded
		 * with this loader
		 */
		bool supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) final;

		/**
		 * Loads a resource.
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return the loaded resource
		 */
		AudioStream::Ptr load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) final;

	};

}



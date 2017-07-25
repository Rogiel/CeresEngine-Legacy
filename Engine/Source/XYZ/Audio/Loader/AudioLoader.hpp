//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Resource/ResourceLoader.hpp"
#include "XYZ/Audio/AudioResource.hpp"

namespace XYZ::Audio::Loader {

	using AudioClipLoader = Resource::ResourceLoader<AudioClip>;
	using AudioStreamLoader = Resource::ResourceLoader<AudioStream>;

}



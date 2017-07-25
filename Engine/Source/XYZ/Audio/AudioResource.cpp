//
// Created by Rogiel Sulzbach on 7/16/17.
//

#include "AudioResource.hpp"

namespace XYZ::Audio {

	AudioResourceType AudioClip::getAudioResourceType() const {
		return AudioResourceType::CLIP;
	}

	bool AbstractAudioResource::isClip() const {
		return getAudioResourceType() == AudioResourceType::CLIP;
	}

	AudioClip& AbstractAudioResource::asClip() {
		if(!isClip()) {
			throw std::bad_cast();
		}
		return *static_cast<AudioClip*>(this);
	}

	const AudioClip& AbstractAudioResource::asClip() const {
		if(!isClip()) {
			throw std::bad_cast();
		}
		return *static_cast<const AudioClip*>(this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	AudioResourceType AudioStream::getAudioResourceType() const {
		return AudioResourceType::STREAM;
	}

	bool AbstractAudioResource::isStream() const {
		return getAudioResourceType() == AudioResourceType::STREAM;
	}

	AudioStream& AbstractAudioResource::asStream() {
		if(!isStream()) {
			throw std::bad_cast();
		}
		return *static_cast<AudioStream*>(this);
	}

	const AudioStream& AbstractAudioResource::asStream() const {
		if(!isStream()) {
			throw std::bad_cast();
		}
		return *static_cast<const AudioStream*>(this);
	}

	// -----------------------------------------------------------------------------------------------------------------

	SimpleAudioClip::SimpleAudioClip(AudioMetadata metadata, Samples samples)
			: metadata(metadata), samples(std::move(samples)) {}

	const AudioMetadata& SimpleAudioClip::getAudioMetadata() const {
		return metadata;
	}

	const AbstractAudioResource::Samples& SimpleAudioClip::getSamples() const {
		return samples;
	}

}
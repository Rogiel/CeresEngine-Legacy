//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include "AudioMetadata.hpp"

namespace XYZ::Audio {

	AudioMetadata::AudioMetadata(AudioMetadata::Channels channels, AudioMetadata::SampleRate sampleRate,
								 SampleCount sampleCount) :
			channels(channels), sampleRate(sampleRate), sampleCount(sampleCount) {}

	AudioMetadata::Channels AudioMetadata::getChannels() const {
		return channels;
	}

	AudioMetadata::SampleRate AudioMetadata::getSampleRate() const {
		return sampleRate;
	}

	AudioMetadata::SampleCount AudioMetadata::getSampleCount() const {
		return sampleCount;
	}
}
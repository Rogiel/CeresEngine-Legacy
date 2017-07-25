//
// Created by Rogiel Sulzbach on 7/16/17.
//

#include "OpenALAudioSystem.hpp"
#include "OpenALAudioBuffer.hpp"

#include <cmath>
#include <iostream>

namespace XYZ::Audio::OpenAL {

	OpenALAudioSystem::OpenALAudioSystem() {
		// Initialization
		device = alcOpenDevice(nullptr);
		if(device == nullptr) {
			return;
		}

		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}

	OpenALAudioSystem::~OpenALAudioSystem() {
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	// -----------------------------------------------------------------------------------------------------------------

	OpenALListener& OpenALAudioSystem::getListener() {
		return listener;
	}

	std::shared_ptr<AudioSource> OpenALAudioSystem::createSource() {
		return std::make_shared<OpenALSource>();
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::shared_ptr<AudioBuffer> OpenALAudioSystem::createAudioBuffer(const AudioClip& clip) {
		return std::make_shared<OpenALAudioBuffer>(clip);
	}
}
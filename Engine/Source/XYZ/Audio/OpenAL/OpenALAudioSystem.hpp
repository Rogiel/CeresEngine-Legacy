//
// Created by Rogiel Sulzbach on 7/16/17.
//

#pragma once

#include "XYZ/Audio/AudioSystem.hpp"

#include <OpenAL/OpenAL.h>

#include "OpenALListener.hpp"
#include "OpenALSource.hpp"

namespace XYZ::Audio::OpenAL {

	class OpenALAudioSystem : public AudioSystem {
	private:
		/**
		 * The OpenAL listener
		 */
		OpenALListener listener;

	private:
		/**
		 * The Mac OS X ALC device
		 */
		ALCdevice* device;

		/**
		 * The Mac OS X ALC context
		 */
		ALCcontext* context;

	public:
		/**
		 * Create a new OpenAL audio system
		 */
		OpenALAudioSystem();

		/**
		 * Destroys the OpenAL audio system
		 */
		~OpenALAudioSystem() final;

	public:
		/**
		 * Gets the audio listener
		 *
		 * @return the audio listener
		 */
		OpenALListener& getListener() final;

		/**
		 * Creates a new audio source
		 *
		 * @return a newly created audio source
		 */
		std::shared_ptr<AudioSource> createSource() final;

	public:
		/**
		 * Create a new AudioBuffer from a audio clip
		 *
		 * @param clip the audio clip to create a buffer from
		 *
		 * @return a newly created audio buffer with the clip contents
		 */
		std::shared_ptr<AudioBuffer> createAudioBuffer(const AudioClip& clip) final;

	};

}



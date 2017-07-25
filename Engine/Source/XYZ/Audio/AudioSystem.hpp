//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include "XYZ/Audio/AudioListener.hpp"
#include "XYZ/Audio/AudioSource.hpp"
#include "AudioResource.hpp"

#include <memory>

namespace XYZ::Audio {

	class AudioSystem {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~AudioSystem() = default;

	public:
		/**
		 * Gets the audio listener
		 *
		 * @return the audio listener
		 */
		virtual AudioListener& getListener() = 0;

		/**
		 * Creates a new audio source
		 *
		 * @return a newly created audio source
		 */
		virtual std::shared_ptr<AudioSource> createSource() = 0;

	public:
		/**
		 * Create a new AudioBuffer from a audio clip
		 *
		 * @param clip the audio clip to create a buffer from
		 *
		 * @return a newly created audio buffer with the clip contents
		 */
		virtual std::shared_ptr<AudioBuffer> createAudioBuffer(const AudioClip& clip) = 0;

	};

}




//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Audio/AudioBuffer.hpp"
#include <OpenAL/OpenAL.h>

#include "XYZ/Audio/AudioResource.hpp"

namespace XYZ::Audio::OpenAL {

	class OpenALAudioBuffer : public AudioBuffer {
	public:
		/**
		 * The OpenAL buffer ID
		 */
		ALuint bufferID;

	public:
		/**
		 * Creates a new unbound OpenAL audio buffer
		 */
		OpenALAudioBuffer();

		/**
		 * Creates a new OpenAL audio buffer from a audio clip
		 */
		explicit OpenALAudioBuffer(const AudioClip& clip);

		/**
		 * Creates a new OpenAL audio buffer by using a already existing handle
		 *
		 * @param bufferID the existing OpenAL buffer handle
		 */
		explicit OpenALAudioBuffer(ALuint bufferID);

		/**
		 * Destroys and releases the OpenAL audio buffer
		 */
		~OpenALAudioBuffer();


	};

}


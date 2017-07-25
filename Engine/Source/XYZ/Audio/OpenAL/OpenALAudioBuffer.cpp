//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include <XYZ/Audio/AudioResource.hpp>
#include "OpenALAudioBuffer.hpp"

namespace XYZ::Audio::OpenAL {

	OpenALAudioBuffer::OpenALAudioBuffer() {
		alGenBuffers(1, &bufferID);
	}

	OpenALAudioBuffer::OpenALAudioBuffer(const AudioClip& clip) : OpenALAudioBuffer() {
		alBufferData(bufferID, AL_FORMAT_MONO16, clip.getSamples().data(), ALsizei(clip.getSamples().size()),
					 clip.getAudioMetadata().getSampleRate());
	}

	OpenALAudioBuffer::OpenALAudioBuffer(ALuint bufferID) :
			bufferID(bufferID) {}

	OpenALAudioBuffer::~OpenALAudioBuffer() {
		if(bufferID != 0) {
			alDeleteBuffers(1, &bufferID);
		}
	}

}
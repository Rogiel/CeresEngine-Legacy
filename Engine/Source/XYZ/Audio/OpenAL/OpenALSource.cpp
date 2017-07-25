//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include "OpenALSource.hpp"
#include "OpenALAudioBuffer.hpp"

#include <iostream>

namespace XYZ::Audio::OpenAL {

	OpenALSource::OpenALSource() {
		alGenSources(1, &sourceID);

		auto origin = glm::vec3(0.0);

		alSourcef(sourceID, AL_PITCH, 1.0f);
		alSourcef(sourceID, AL_GAIN, 1.0f);
		alSourcefv(sourceID, AL_POSITION, &origin[0]);
		alSourcefv(sourceID, AL_VELOCITY, &origin[0]);
		alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	}

	OpenALSource::OpenALSource(ALuint sourceID) : sourceID(sourceID) {}

	OpenALSource::~OpenALSource() {
		if(sourceID != 0) {
			alDeleteSources(1, &sourceID);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenALSource::play(const AudioBuffer& rawBuffer) {
		auto& buffer = static_cast<const OpenALAudioBuffer&>(rawBuffer);
		currentBuffer = rawBuffer.shared_from_this();

		alSourcei(sourceID, AL_BUFFER, buffer.bufferID);
		alSourcePlay(sourceID);
	}

	bool OpenALSource::isPlaying() {
		ALenum state;
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	void OpenALSource::stop() {
		alSourceStop(sourceID);
	}

	bool OpenALSource::isStopped() {
		ALenum state;
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_STOPPED);
	}

	void OpenALSource::pause() {
		alSourcePause(sourceID);
	}

	bool OpenALSource::isPaused() {
		ALenum state;
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PAUSED);
	}

	void OpenALSource::resume() {
		alSourcePlay(sourceID);
	}

	// -----------------------------------------------------------------------------------------------------------------

	glm::vec3 OpenALSource::getPosition() const {
		glm::vec3 position;
		alGetSourcefv(sourceID, AL_POSITION, &position[0]);
		return position;
	}

	void OpenALSource::setPosition(const glm::vec3& position) {
		alSourcefv(sourceID, AL_POSITION, &position[0]);
	}

	glm::vec3 OpenALSource::getDirection() const {
		glm::vec3 direction;
		alGetSourcefv(sourceID, AL_DIRECTION, &direction[0]);
		return direction;
	}

	void OpenALSource::setDirection(const glm::vec3& direction) {
		alSourcefv(sourceID, AL_DIRECTION, &direction[0]);
	}

	glm::vec3 OpenALSource::getVelocity() const {
		glm::vec3 velocity;
		alGetSourcefv(sourceID, AL_VELOCITY, &velocity[0]);
		return velocity;
	}

	void OpenALSource::setVelocity(const glm::vec3& velocity) {
		alSourcefv(sourceID, AL_VELOCITY, &velocity[0]);
	}

	bool OpenALSource::isLooping() const {
		bool looping;
		alGetSourcei(sourceID, AL_LOOPING, (ALint*) &looping);
		return looping;
	}

	void OpenALSource::setLooping(bool looping) {
		alSourcei(sourceID, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}

	float OpenALSource::getGain() const {
		float gain;
		alGetSourcef(sourceID, AL_GAIN, &gain);
		return gain;
	}

	void OpenALSource::setGain(float gain) {
		alSourcef(sourceID, AL_GAIN, gain);
	}

	float OpenALSource::getMinimumGain() const {
		float minGain;
		alGetSourcef(sourceID, AL_MIN_GAIN, &minGain);
		return minGain;
	}

	void OpenALSource::setMinimumGain(float minimumGain) {
		alSourcef(sourceID, AL_MIN_GAIN, minimumGain);
	}

	float OpenALSource::getMaximumGain() const {
		float maxGain;
		alGetSourcef(sourceID, AL_MAX_GAIN, &maxGain);
		return maxGain;
	}

	void OpenALSource::setMaximumGain(float maximumGain) {
		alSourcef(sourceID, AL_MAX_GAIN, maximumGain);
	}

	float OpenALSource::getPitch() const {
		float pitch;
		alGetSourcef(sourceID, AL_PITCH, &pitch);
		return pitch;
	}

	void OpenALSource::setPitch(float pitch) {
		alSourcef(sourceID, AL_PITCH, pitch);
	}

}
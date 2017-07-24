//
// Created by Rogiel Sulzbach on 7/18/17.
//

#include "AudioSource.hpp"

namespace XYZ::Audio {


	const glm::vec3& AudioSource::getPosition() const {
		return position;
	}

	void AudioSource::setPosition(const glm::vec3& position) {
		AudioSource::position = position;
	}

	const glm::vec3& AudioSource::getDirection() const {
		return direction;
	}

	void AudioSource::setDirection(const glm::vec3& direction) {
		AudioSource::direction = direction;
	}

	const glm::vec3& AudioSource::getVelocity() const {
		return velocity;
	}

	void AudioSource::setVelocity(const glm::vec3& velocity) {
		AudioSource::velocity = velocity;
	}

	bool AudioSource::isLooping() const {
		return looping;
	}

	void AudioSource::setLooping(bool looping) {
		AudioSource::looping = looping;
	}

	float AudioSource::getGain() const {
		return gain;
	}

	void AudioSource::setGain(float gain) {
		AudioSource::gain = gain;
	}

	float AudioSource::getMinimumGain() const {
		return minimumGain;
	}

	void AudioSource::setMinimumGain(float minimumGain) {
		AudioSource::minimumGain = minimumGain;
	}

	float AudioSource::getMaximumGain() const {
		return maximumGain;
	}

	void AudioSource::setMaximumGain(float maximumGain) {
		AudioSource::maximumGain = maximumGain;
	}

	float AudioSource::getPitch() const {
		return pitch;
	}

	void AudioSource::setPitch(float pitch) {
		AudioSource::pitch = pitch;
	}
}

//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include "OpenALListener.hpp"

namespace XYZ::Audio::OpenAL {

	OpenALListener::OpenALListener() = default;

	OpenALListener::~OpenALListener() = default;

	// -----------------------------------------------------------------------------------------------------------------

	glm::vec3 OpenALListener::getPosition() const {
		glm::vec3 position;
		alGetListenerfv(AL_POSITION, &position[0]);
		return position;
	}

	void OpenALListener::setPosition(const glm::vec3& position) {
		alListenerfv(AL_POSITION, &position[0]);
	}

	glm::vec3 OpenALListener::getDirection() const {
		glm::vec3 direction;
		alGetListenerfv(AL_DIRECTION, &direction[0]);
		return direction;
	}

	void OpenALListener::setDirection(const glm::vec3& direction) {
		alListenerfv(AL_DIRECTION, &direction[0]);
	}

	glm::vec3 OpenALListener::getVelocity() const {
		glm::vec3 velocity;
		alGetListenerfv(AL_VELOCITY, &velocity[0]);
		return velocity;
	}

	void OpenALListener::setVelocity(const glm::vec3& velocity) {
		alListenerfv(AL_VELOCITY, &velocity[0]);
	}

}

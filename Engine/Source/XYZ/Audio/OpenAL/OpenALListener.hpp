//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Audio/AudioListener.hpp"
#include <OpenAL/OpenAL.h>

namespace XYZ::Audio::OpenAL {

	class OpenALListener : public AudioListener {
	public:
		/**
		 * Create a new OpenAL listener
		 */
		OpenALListener();

		/**
		 * Destroys the OpenAL listener
		 */
		~OpenALListener() final;

	public:
		/**
		 * @return the listener position
		 */
		glm::vec3 getPosition() const final;

		/**
		 * Sets the listener position
		 *
		 * @param position the listener position
		 */
		void setPosition(const glm::vec3& position) final;

		/**
		 * @return the listener facing direction
		 */
		glm::vec3 getDirection() const final;

		/**
		 * @param direction the listener facing direction
		 */
		void setDirection(const glm::vec3& direction) final;

		/**
		 * @return the listener moving velocity in the set direction
		 */
		glm::vec3 getVelocity() const final;

		/**
		 * @param velocity the listener moving velocity in the set direction
		 */
		void setVelocity(const glm::vec3& velocity) final;
		
	};

}




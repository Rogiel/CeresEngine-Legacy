//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include <glm/glm.hpp>

namespace XYZ::Audio {

	/**
	 * A listener is a entity that represents the location, direction
	 * and velocity of a object (generally the player) that is
	 * listening to audio being emitted by one or more sources.
	 */
	class AudioListener {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~AudioListener() = default;

	public:
		/**
		 * @return the listener position
		 */
		virtual glm::vec3 getPosition() const = 0;

		/**
		 * Sets the listener position
		 *
		 * @param position the listener position
		 */
		virtual void setPosition(const glm::vec3& position) = 0;

		/**
		 * @return the listener facing direction
		 */
		virtual glm::vec3 getDirection() const = 0;

		/**
		 * @param direction the listener facing direction
		 */
		virtual void setDirection(const glm::vec3& direction) = 0;

		/**
		 * @return the listener moving velocity in the set direction
		 */
		virtual glm::vec3 getVelocity() const = 0;

		/**
		 * @param velocity the listener moving velocity in the set direction
		 */
		virtual void setVelocity(const glm::vec3& velocity) = 0;

	};

}


//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include "XYZ/Audio/AudioBuffer.hpp"

#include <glm/glm.hpp>

namespace XYZ::Audio {

	/**
	 * A audio source is a entity that emmits sounds.
	 *
	 * A source can have several attributes set for it, including
	 * position, direction, velocity, gain, etc.
	 *
	 * A source can only play a single audio resource at once. If
	 * a object must emmit two or more sounds at once, more sources
	 * must be created and attached to the object.
	 */
	class AudioSource {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~AudioSource() = default;

	public:
		/**
		 * Plays a audio buffer
		 *
		 * @param buffer the audio buffer to be played
		 */
		virtual void play(const AudioBuffer& buffer) = 0;

		/**
		 * @return true if the source is busy playing a audio
		 */
		virtual bool isPlaying() = 0;

		/**
		 * Stops playing sound from the source. If there is no
		 * sound being played, this method does nothing.
		 */
		virtual void stop() = 0;

		/**
		 * Checks if the audio source is already stopped.
		 *
		 * @return true if the source is not playing anything
		 */
		virtual bool isStopped() = 0;

		/**
		 * Pauses the audio source. Playback can be resumed
		 * from the same point with a call to <tt>resume()</tt>.
		 */
		virtual void pause() = 0;

		/**
		 * Checks if the audio source reproduction is paused.
		 *
		 * @return true if the source reproduction is paused
		 */
		virtual bool isPaused() = 0;

		/**
		 * Resumes playing audio tracks.
		 */
		virtual void resume() = 0;

	public:
		/**
		 * @return the source position
		 */
		virtual glm::vec3 getPosition() const = 0;

		/**
		 * @param position the source position
		 */
		virtual void setPosition(const glm::vec3& position) = 0;

		/**
		 * @return the source direction
		 */
		virtual glm::vec3 getDirection() const = 0;

		/**
		 * @param direction the source direction
		 */
		virtual void setDirection(const glm::vec3& direction) = 0;

		/**
		 * @return the source velocity
		 */
		virtual glm::vec3 getVelocity() const = 0;

		/**
		 * @param velocity the source velocity
		 */
		virtual void setVelocity(const glm::vec3& velocity) = 0;

		/**
		 * @return true if the audio resource is to be looped when playing
		 */
		virtual bool isLooping() const = 0;

		/**
		 * @param looping true if the audio resource is to be looped when playing
		 */
		virtual void setLooping(bool looping) = 0;

		/**
		 * @return the source gain
		 */
		virtual float getGain() const = 0;

		/**
		 * @param gain the source gain
		 */
		virtual void setGain(float gain) = 0;

		/**
		 * @return the source minimum gain
		 */
		virtual float getMinimumGain() const = 0;

		/**
		 * @param minimumGain the source minimum gain
		 */
		virtual void setMinimumGain(float minimumGain) = 0;

		/**
		 * @return the source maximum gain
		 */
		virtual float getMaximumGain() const = 0;

		/**
		 * @param maximumGain the source maximum gain
		 */
		virtual void setMaximumGain(float maximumGain) = 0;

		/**
		 * @return the source pitch
		 */
		virtual float getPitch() const = 0;

		/**
		 * @param pitch the source pitch
		 */
		virtual void setPitch(float pitch) = 0;


	};

}


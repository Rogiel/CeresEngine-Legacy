//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Audio/AudioSource.hpp"
#include <OpenAL/OpenAL.h>

namespace XYZ::Audio::OpenAL {

	class OpenALSource : public AudioSource {
	private:
		/**
		 * The OpenAL source ID
		 */
		ALuint sourceID;

		/**
		 * The audio buffer currently being played
		 */
		std::shared_ptr<const AudioBuffer> currentBuffer;

	public:
		/**
		 * Create a new OpenAL source
		 */
		OpenALSource();

		/**
		 * Create a new OpenAL source with an existing handle
		 *
		 * @param sourceID the existing source handle
		 */
		OpenALSource(ALuint sourceID);

		/**
		 * Destroys the OpenAL source object
		 */
		~OpenALSource() final;

	public:
		/**
		 * Plays a audio buffer
		 *
		 * @param buffer the audio buffer to be played
		 */
		void play(const AudioBuffer& buffer) final;

		/**
		 * @return true if the source is busy playing a audio
		 */
		bool isPlaying() final;

		/**
		 * Stops playing sound from the source. If there is no
		 * sound being played, this method does nothing.
		 */
		void stop() final;

		/**
		 * Checks if the audio source is already stopped.
		 *
		 * @return true if the source is not playing anything
		 */
		bool isStopped() final;

		/**
		 * Pauses the audio source. Playback can be resumed
		 * from the same point with a call to <tt>resume()</tt>.
		 */
		void pause() final;

		/**
		 * Checks if the audio source reproduction is paused.
		 *
		 * @return true if the source reproduction is paused
		 */
		bool isPaused() final;

		/**
		 * Resumes playing audio tracks.
		 */
		void resume() final;

	public:
		/**
		 * @return the source position
		 */
		glm::vec3 getPosition() const final;

		/**
		 * @param position the source position
		 */
		void setPosition(const glm::vec3& position) final;

		/**
		 * @return the source direction
		 */
		glm::vec3 getDirection() const final;

		/**
		 * @param direction the source direction
		 */
		void setDirection(const glm::vec3& direction) final;

		/**
		 * @return the source velocity
		 */
		glm::vec3 getVelocity() const final;

		/**
		 * @param velocity the source velocity
		 */
		void setVelocity(const glm::vec3& velocity) final;

		/**
		 * @return true if the audio resource is to be looped when playing
		 */
		bool isLooping() const final;

		/**
		 * @param looping true if the audio resource is to be looped when playing
		 */
		void setLooping(bool looping) final;

		/**
		 * @return the source gain
		 */
		float getGain() const final;

		/**
		 * @param gain the source gain
		 */
		void setGain(float gain) final;

		/**
		 * @return the source minimum gain
		 */
		float getMinimumGain() const final;

		/**
		 * @param minimumGain the source minimum gain
		 */
		void setMinimumGain(float minimumGain) final;

		/**
		 * @return the source maximum gain
		 */
		float getMaximumGain() const final;

		/**
		 * @param maximumGain the source maximum gain
		 */
		void setMaximumGain(float maximumGain) final;

		/**
		 * @return the source pitch
		 */
		float getPitch() const final;

		/**
		 * @param pitch the source pitch
		 */
		void setPitch(float pitch) final;

	};

}


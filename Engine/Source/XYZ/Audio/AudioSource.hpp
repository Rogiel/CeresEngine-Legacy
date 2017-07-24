//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include "XYZ/Audio/AudioResource.hpp"

#include <glm/glm.hpp>

namespace XYZ::Audio {

	class AudioSource {
	private:
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 velocity;

		bool looping;

		float gain;
		float minimumGain;
		float maximumGain;

		float pitch;

	public:
		virtual void play(std::shared_ptr<AudioResource> audioResource) = 0;
		virtual bool isPlaying() = 0;

		virtual void stop() = 0;
		virtual bool isStopped() = 0;

		virtual void pause() = 0;
		virtual bool isPaused() = 0;

		virtual void resume() = 0;

	public:
		const glm::vec3& getPosition() const;
		void setPosition(const glm::vec3& position);

		const glm::vec3& getDirection() const;
		void setDirection(const glm::vec3& direction);

		const glm::vec3& getVelocity() const;
		void setVelocity(const glm::vec3& velocity);

		bool isLooping() const;
		void setLooping(bool looping);

		float getGain() const;
		void setGain(float gain);

		float getMinimumGain() const;
		void setMinimumGain(float minimumGain);

		float getMaximumGain() const;
		void setMaximumGain(float maximumGain);

		float getPitch() const;
		void setPitch(float pitch);


	};

}


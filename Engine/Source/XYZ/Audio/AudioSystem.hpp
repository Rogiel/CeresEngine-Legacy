//
// Created by Rogiel Sulzbach on 7/18/17.
//

#pragma once

#include "XYZ/Audio/AudioListener.hpp"
#include "XYZ/Audio/AudioSource.hpp"

namespace XYZ::Audio {

	class AudioSystem {
	public:
		/**
		 * Creates a new audio listener.
		 *
		 * @return a newly created audio listener
		 */
		virtual std::shared_ptr<AudioListener> createListener() = 0;

		/**
		 * Creates a new audio source
		 *
		 * @return a newly created audio source
		 */
		virtual std::shared_ptr<AudioSource> createSource() = 0;

	};

}




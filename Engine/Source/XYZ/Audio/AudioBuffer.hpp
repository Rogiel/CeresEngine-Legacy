//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

namespace XYZ::Audio {

	/**
	 * The AudioBuffer class represents a buffer (or a piece of) of audio
	 * samples that are loaded into a audio engine.
	 *
	 * These samples can be directly played by a device.
	 */
	class AudioBuffer : public Resource::Resource<AudioBuffer> {

	};

}

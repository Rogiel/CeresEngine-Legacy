//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

#include "XYZ/Audio/AudioResource.hpp"

namespace XYZ::Audio::Loader::OggVorbis {

	class OggVorbisAudioStream : public AudioStream {
	public:
		/**
		 * Reads up to <tt>samples</tt> from the stream.
		 *
		 * @param samples the maximum number of samples to be read
		 * @param callback the callback to be called after the operation
		 * has completed
		 */
		void readSamples(unsigned int samples, ReadSamplesCallback&& callback) final;

		/**
		 * Seeks the audio stream
		 *
		 * @param index the sample to seek the stream to
		 */
		void seek(SampleIndex index) final;

	};

}


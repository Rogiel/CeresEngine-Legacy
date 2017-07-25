//
// Created by Rogiel Sulzbach on 7/24/17.
//

#pragma once

namespace XYZ::Audio {

	/**
	 * A class collecting audio metadata information
	 */
	class AudioMetadata {
	public:
		/**
		 * A type defining the channel count representation
		 */
		using Channels = unsigned int;

		/**
		 * A type defining the sample rate representation
		 */
		using SampleRate = unsigned int;

		/**
		 * A type defining the number of counts representation
		 */
		using SampleCount = unsigned int;

	private:
		/**
		 * The number of channels in the audio
		 */
		Channels channels;

		/**
		 * The audio clip sample rate in Hz
		 */
		SampleRate sampleRate;

		/**
		 * The number of samples in the audio
		 */
		SampleCount sampleCount;

	public:
		/**
		 * Creates a new audio metadata object
		 *
		 * @param channels the number of audio channels
		 * @param sampleRate the audio sample rate
		 * @param sampleCount the number of samples in the audio
		 */
		AudioMetadata(Channels channels, SampleRate sampleRate, SampleCount sampleCount);

	public:
		/**
		 * @return the number of channels in the audio clip
		 */
		Channels getChannels() const;

		/**
		 * @return the audio clip sample rate in Hz
		 */
		SampleRate getSampleRate() const;

		/**
		 * @return the number of samples in the audio
		 */
		SampleCount getSampleCount() const;

	};

}


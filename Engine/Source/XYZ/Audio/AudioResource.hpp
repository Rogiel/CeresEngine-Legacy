//
// Created by Rogiel Sulzbach on 7/16/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"
#include "XYZ/Audio/AudioMetadata.hpp"

#include <vector>
#include <memory>

namespace XYZ::Audio {

	class AudioClip;
	class AudioStream;

	enum class AudioResourceType {
		/**
		 * A audio clip is a small piece of audio that is entirely loaded
	 	 * into memory.
		 */
				CLIP,

		/**
		 * A audio stream is a larger piece of audio that will be streammed
		 * for the device (instead of feeding all audio samples all at once).
		 */
				STREAM
	};

	/**
	 * A abstract audio resource.
	 *
	 * This class represents both a audio clip and a
	 * audio stream.
	 */
	class AbstractAudioResource {
	public:
		/**
		 * A audio sample type
		 */
		using Sample = unsigned short;

		/**
		 * A collection of multiple audio samples
		 */
		using Samples = std::vector<Sample>;

		/**
		 * A type representing the index of a sample
		 */
		using SampleIndex = unsigned int;

		/**
		 * A type representing a sample offset
		 */
		using SampleOffset = int;

	public:
		/**
		 * @return the audio resource metadata
		 */
		virtual const AudioMetadata& getAudioMetadata() const = 0;

	public:
		/**
		 * @return the audio resource type
		 */
		virtual AudioResourceType getAudioResourceType() const = 0;

		/**
		 * @return true if the audio resource is a clip
		 */
		bool isClip() const;

		/**
		 * @return the audio resource as a clip
		 *
		 * NOTE: this method throws if <tt>isClip()</tt> returns false.
		 */
		AudioClip& asClip();

		/**
		 * @return the audio resource as a clip
		 *
		 * NOTE: this method throws if <tt>isClip()</tt> returns false.
		 */
		const AudioClip& asClip() const;

		/**
		 * @return true if the audio resource is a stream
		 */
		bool isStream() const;

		/**
		 * @return the audio resource as a stream
		 *
		 * NOTE: this method throws if <tt>isStream()</tt> returns false.
		 */
		AudioStream& asStream();

		/**
		 * @return the audio resource as a stream
		 *
		 * NOTE: this method throws if <tt>isStream()</tt> returns false.
		 */
		const AudioStream& asStream() const;

	};

	/**
	 * A audio resource. This resource can be templated on
	 * <tt>T</tt> with a audio resource specialization
	 *
	 * @tparam T the specialized audio resource type
	 * @tparam S the audio sample type
	 */
	template<typename T>
	class AudioResource : public AbstractAudioResource,
						  public Resource::Resource<T> {

	};

	/**
	 * A audio clip is a small piece of audio that is entirely loaded
	 * into memory.
	 *
	 * In most cases, a clip is so small that it would not make sense
	 * (and even be slower!) to stream the audio in pieces.
	 *
	 * Audio clips are most suitable for audio effects and tracks that
	 * are played frequently or multiple times.
	 *
	 * @tparam S the audio sample type
	 */
	class AudioClip : public AudioResource<AudioClip> {
	public:
		/**
		 * @return the audio clip samples
		 */
		virtual const Samples& getSamples() const = 0;

	public:
		/**
		 * @return always return CLIP
		 */
		AudioResourceType getAudioResourceType() const final;
	};

	/**
	 * A audio stream is a larger piece of audio that will be streammed
	 * for the device (instead of feeding all audio samples all at once).
	 *
	 * Audio streams are suitable for background music, VOIP data (which
	 * by nature is a stream) or any other larger audio track that is
	 * large and the implementer does not want to load it entirely on RAM.
	 */
	class AudioStream : public AudioResource<AudioStream> {
	public:
		/**
		 * A callback used as a reply for a read samples operation
		 */
		using ReadSamplesCallback = std::function<void(
				/**
				 * The audio stream generating the reply
				 */
				AudioStream&,

				/**
				 * A error code indicating the erron condition, if any,
				 * has occured
				 */
				std::error_code,

				/**
				 * A vector of the read samples. This buffer is safe to be moved.
				 */
				Samples&
		)>;

	public:
		/**
		 * Reads up to <tt>samples</tt> from the stream.
		 *
		 * @param samples the maximum number of samples to be read
		 * @param callback the callback to be called after the operation
		 * has completed
		 */
		virtual void readSamples(unsigned int samples, ReadSamplesCallback&& callback) = 0;

		/**
		 * Seeks the audio stream
		 *
		 * @param index the sample to seek the stream to
		 */
		virtual void seek(SampleIndex index) = 0;

	public:
		/**
		 * @return always return STREAM
		 */
		AudioResourceType getAudioResourceType() const final;

	};

	// -----------------------------------------------------------------------------------------------------------------

	class SimpleAudioClip : public AudioClip {
	public:
		/**
		 * The audio metadata object
		 */
		AudioMetadata metadata;

		/**
		 * A vector containing the audio samples as 16 bit integers
		 */
		Samples samples;

	public:
		/**
		 * Creates a new audio clip
		 *
		 * @param metadata the audio metadata object
		 * @param samples a vector containing the audio samples as 16 bit integers
		 */
		SimpleAudioClip(AudioMetadata metadata, Samples samples);

	public:
		/**
		 * @return the audio resource metadata
		 */
		const AudioMetadata& getAudioMetadata() const override;

		/**
		 * @return the audio clip samples
		 */
		const Samples& getSamples() const override;
	};

}




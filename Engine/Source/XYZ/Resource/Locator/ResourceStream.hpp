//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include <cstdint>
#include <cstddef>

#include <streambuf>
#include <memory>

namespace XYZ::Resource::Locator {

	enum class ResourceStreamSeekType {

		/**
		 * Sets the absolute position of the stream
		 */
				SET,

		/**
		 * Increments the stream position
		 */
				CURRENT_POSITON,

	};

	/**
	 * A ResourceStream object allows a user to read from a located resource
	 */
	class ResourceStream {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~ResourceStream() = default;

	public:
		/**
		 * Reads <tt>len</tt> bytes from the resource stream
		 *
		 * @param bytes the buffer to read to
		 * @param len the amount of bytes to read
		 *
		 * @return the number of bytes effectively read
		 */
		virtual std::streamsize read(uint8_t* bytes, std::streamsize len) = 0;

		/**
		 * Seeks the stream to the position <tt>position</tt>.
		 *
		 * @param seek the position to seek to or with
		 * @param type the seeking type
		 */
		virtual void seek(std::streamsize seek, ResourceStreamSeekType type = ResourceStreamSeekType::SET) = 0;

		/**
		 * @return the current stream position
		 */
		virtual std::streamsize tell() = 0;

		/**
		 * Checks if a stream has any remaining data
		 *
		 * @return true if read() will return at least 1 byte
		 */
		virtual bool hasData() = 0;
	};

	/**
	 * A streambuf implementation for a ResourceStream
	 */
	class ResourceStreamBuf : public std::streambuf {
	private:
		/**
		 * The resource stream
		 */
		std::unique_ptr<ResourceStream> stream;

		/**
		 * The resource stream buffer
		 */
		char buffer[1024];

	public:
		/**
		 * Creates a new ResourceStream buffer
		 *
		 * @param stream the ResourceStream instance
		 */
		ResourceStreamBuf(std::unique_ptr<ResourceStream> stream);

		/**
		 * Releases the ResourceStream
		 */
		virtual ~ResourceStreamBuf();

	protected:
		int underflow() override;
	};

}



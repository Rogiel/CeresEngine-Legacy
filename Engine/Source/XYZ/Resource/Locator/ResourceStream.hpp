//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include <cstdint>
#include <cstddef>

#include <streambuf>
#include <memory>

namespace XYZ::Resource::Locator {

	/**
	 * A ResourceStream object allows a user to read from a located resource
	 */
	class ResourceStream {
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



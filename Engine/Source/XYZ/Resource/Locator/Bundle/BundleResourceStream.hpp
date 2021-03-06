//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include <CoreFoundation/CoreFoundation.h>
#include "XYZ/Resource/Locator/ResourceStream.hpp"

#include <cstdint>

namespace XYZ::Resource::Locator::Bundle {

	class BundleResourceStream : public ResourceStream {
	private:
		/**
		 * The read stream
		 */
		CFReadStreamRef stream;

	public:
		/**
		 * Creates a new BundleResourceStream from a read stream
		 *
		 * @param stream the read stream
		 */
		explicit BundleResourceStream(CFReadStreamRef stream);

		/**
		 * Releases the read stream
		 */
		virtual ~BundleResourceStream();

	public:
		/**
		 * Reads <tt>len</tt> bytes from the resource stream
		 *
		 * @param bytes the buffer to read to
		 * @param len the amount of bytes to read
		 *
		 * @return the number of bytes effectively read
		 */
		std::streamsize read(uint8_t* bytes, std::streamsize len) final;

		/**
		 * Seeks the stream to the position <tt>position</tt>.
		 *
		 * @param seek the position to seek to or with
		 * @param type the seeking type
		 */
		void seek(std::streamsize seek, ResourceStreamSeekType type = ResourceStreamSeekType::SET) final;

		/**
		 * @return the current stream position
		 */
		std::streamsize tell() final;

		/**
		 * Checks if a stream has any remaining data
		 *
		 * @return true if read() will return at least 1 byte
		 */
		bool hasData() final;
	};

}



//
// Created by Rogiel Sulzbach on 7/14/17.
//

#include "ResourceStream.hpp"

#include <iostream>

namespace XYZ::Resource::Locator {

	ResourceStreamBuf::ResourceStreamBuf(std::unique_ptr<ResourceStream> stream) : stream(std::move(stream)) {}

	ResourceStreamBuf::~ResourceStreamBuf() = default;

	// -----------------------------------------------------------------------------------------------------------------

	int ResourceStreamBuf::underflow() {
		if(!stream->hasData()) {
			return std::char_traits<char>::eof();
		}

		if(this->gptr() != this->egptr()) {
			return std::char_traits<char>::to_int_type(*this->gptr());
		}

		// read from the stream and return it
		std::streamsize size = stream->read((uint8_t*) buffer, sizeof(buffer));
		if(size == 0) {
			return std::char_traits<char>::eof();
		}

		this->setg(buffer, buffer, buffer + size);

		return std::char_traits<char>::to_int_type(*this->gptr());
	}

}
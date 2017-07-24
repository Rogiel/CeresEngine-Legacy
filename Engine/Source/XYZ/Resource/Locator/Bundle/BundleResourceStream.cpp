//
// Created by Rogiel Sulzbach on 7/14/17.
//

#include "BundleResourceStream.hpp"

namespace XYZ::Resource::Locator::Bundle {

	BundleResourceStream::BundleResourceStream(CFReadStreamRef stream) : stream(stream) {}

	BundleResourceStream::~BundleResourceStream() {
		if(stream != nullptr) {
			CFReadStreamClose(stream);
			CFRelease(stream);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::streamsize BundleResourceStream::read(uint8_t* bytes, std::streamsize len) {
		return CFReadStreamRead(stream, bytes, len);
	}

	bool BundleResourceStream::hasData() {
		return CFReadStreamHasBytesAvailable(stream) == TRUE;
	}

}
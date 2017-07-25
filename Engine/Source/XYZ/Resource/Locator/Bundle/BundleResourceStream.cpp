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

	void BundleResourceStream::seek(std::streamsize seek, ResourceStreamSeekType type) {
		if(type == ResourceStreamSeekType::CURRENT_POSITON) {
			seek = tell() + seek;
		}

		auto offsetNr = CFNumberCreate(nullptr, kCFNumberLongLongType, &seek);
		CFReadStreamSetProperty(stream, kCFStreamPropertyFileCurrentOffset, offsetNr);
		CFRelease(offsetNr);
	}

	std::streamsize BundleResourceStream::tell() {
		auto offsetNr = static_cast<CFNumberRef>(CFReadStreamCopyProperty(stream, kCFStreamPropertyFileCurrentOffset));
		std::streamsize offset = 0;
		CFNumberGetValue(offsetNr, kCFNumberLongLongType, &offset);
		CFRelease(offsetNr);
		return offset;
	}

	bool BundleResourceStream::hasData() {
		return CFReadStreamHasBytesAvailable(stream) == TRUE;
	}

}
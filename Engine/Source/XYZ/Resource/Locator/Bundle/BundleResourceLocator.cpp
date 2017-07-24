//
// Created by Rogiel Sulzbach on 7/2/17.
//

#include "BundleResourceLocator.hpp"
#include "BundleResourceStream.hpp"

#include <iostream>

namespace XYZ::Resource::Locator::Bundle {

	BundleResourceLocator::BundleResourceLocator() :
			BundleResourceLocator(CFBundleGetMainBundle()) {}

	BundleResourceLocator::BundleResourceLocator(CFBundleRef bundle) :
			bundle(bundle) {
		CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);
		char path[PATH_MAX];
		if(CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*) path, PATH_MAX) == FALSE) {
			CFRelease(resourcesURL);
			throw std::runtime_error("Invalid bundle!");
		}
		CFRelease(resourcesURL);
	}

	BundleResourceLocator::~BundleResourceLocator() {
		CFRelease(bundle);
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::unique_ptr<ResourceStream> BundleResourceLocator::locate(const std::string& resourceName) {
		CFURLRef resourceURL = getResourceURL(resourceName);
		if(resourceURL == nullptr) {
			return nullptr;
		}

		CFReadStreamRef readStream = CFReadStreamCreateWithFile(nullptr, resourceURL);
		if(readStream == nullptr) {
			CFRelease(resourceURL);
			return nullptr;
		}

		CFReadStreamOpen(readStream);
		CFRelease(resourceURL);

		return std::make_unique<BundleResourceStream>(readStream);
	}

	std::string BundleResourceLocator::getResourcePath(const std::string& resourceName) {
		CFURLRef resourceURL = getResourceURL(resourceName);

		char path[PATH_MAX];
		if (CFURLGetFileSystemRepresentation(resourceURL, TRUE, (UInt8 *)path, PATH_MAX) == FALSE) {
			CFRelease(resourceURL);
			throw std::runtime_error("Invalid bundle!");
		}

		std::string str = std::string(path);
		CFRelease(resourceURL);

		return str;
	}

	// -----------------------------------------------------------------------------------------------------------------

	CFURLRef BundleResourceLocator::getResourceURL(const std::string& resourceName) {
		CFStringRef resourceNameCFString = CFStringCreateWithCString(nullptr, resourceName.data(), kCFStringEncodingUTF8);
		CFURLRef url = CFBundleCopyResourceURL(bundle, resourceNameCFString, nullptr, nullptr);
		CFRelease(resourceNameCFString);
		return url;
	}

}

//
// Created by Rogiel Sulzbach on 7/2/17.
//

#pragma once

#include <CoreFoundation/CoreFoundation.h>
#include "XYZ/Resource/Locator/ResourceLocator.hpp"

namespace XYZ::Resource::Locator::Bundle {

	class BundleResourceLocator : public ResourceLocator {
	private:
		/**
		 * The bundle to load resources from
		 */
		CFBundleRef bundle;

	public:
		/**
		 * Creates a new bundle resource locator
		 */
		BundleResourceLocator();

		/**
		 * Creates a new bundle resource locator using a given bundle
		 *
		 * @param bundle the bundle to load resources from
		 */
		explicit BundleResourceLocator(CFBundleRef bundle);

		/**
		 * Destroys a bundle resource locator
		 */
		~BundleResourceLocator();

	public:
		/**
		 * Locates a resource by its name
		 *
		 * @param resourceName the resource name
		 *
		 * @return the located resource
		 */
		std::unique_ptr<ResourceStream> locate(const std::string& resourceName) override;

		/**
		 * Locates a resource by its name and return the local path as a string
		 *
		 * @param resourceName the resource name
		 *
		 * @return the located resource path
		 */
		std::string getResourcePath(const std::string& resourceName);

	private:
		/**
		 * Locates a resource by its name and return its URL
		 *
		 * @param resourceName the resource name
		 *
		 * @return the located resource URL
		 */
		CFURLRef getResourceURL(const std::string& resourceName);
	};

}



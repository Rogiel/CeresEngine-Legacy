//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include "ResourceStream.hpp"

#include <string>
#include <memory>

namespace XYZ::Resource::Locator {

	class ResourceLocator {
	public:
		/**
		 * Locates a resource by its name
		 *
		 * @param resourceName the resource name
		 *
		 * @return the located resource
		 */
		virtual std::unique_ptr<ResourceStream> locate(const std::string& resourceName) = 0;

	};

}



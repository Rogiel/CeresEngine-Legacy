//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include "Resource.hpp"
#include "XYZ/Resource/Locator/ResourceStream.hpp"

#include <memory>

namespace XYZ::Resource {

	template<typename Resource, typename Input1 = std::unique_ptr<Locator::ResourceStream>,
			typename... Inputs>
	class ResourceLoader {
	public:
		/**
		 * Checks if the given resource input is supported by the loader
		 *
		 * @param resourceInput the first resource input
		 * @param remainingInputs the remaining inputs
		 *
		 * @return true if the resource is supported and can be loaded
		 * with this loader
		 */
		virtual bool supports(const Input1& resourceInput, const Inputs& ... remainingInputs) = 0;

		/**
		 * Loads a resource.
		 *
		 * @param resourceInput the first resource input
		 * @param remainingInputs the remaining inputs
		 *
		 * @return the loaded resource
		 */
		virtual typename Resource::Ptr load(Input1 resourceInput, Inputs ... remainingInputs) = 0;

	};

}



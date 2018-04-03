//
// Created by Rogiel Sulzbach on 7/30/17.
//

#pragma once

#include "XYZ/Resource/ResourceLoader.hpp"
#include "XYZ/Graphics/Model/Model.hpp"

namespace XYZ::Graphics::Model::Loader {

	class ModelLoader : public Resource::ResourceLoader<Model> {
	public:
		/**
		 * Checks if the given resource input is supported by the loader
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return true if the resource is supported and can be loaded
		 * with this loader
		 */
		bool supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) override = 0;

		/**
		 * Loads a resource.
		 *
		 * @param resourceStream the resource stream
		 *
		 * @return the loaded resource
		 */
		Model::Ptr load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) override = 0;
	};

}


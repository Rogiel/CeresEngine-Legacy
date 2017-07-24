//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "XYZ/Resource/Locator/ResourceLocator.hpp"
#include "XYZ/Resource/ResourceLoader.hpp"

#include <iostream>

namespace XYZ::Resource {

	template<typename T, typename Loader = ResourceLoader <T>>
	class ResourceManager {
	protected:
		/**
		 * The list of currently active resources
		 */
		std::unordered_map<std::string, std::shared_ptr<T>> resources;

		/**
		 * The list of resource loaders
		 */
		std::vector<std::unique_ptr<Loader>> resourceLoaders;

		/**
		 * The resource locator
		 */
		std::shared_ptr<Locator::ResourceLocator> resourceLocator;

		/**
		 * The prefix path to load resources from
		 */
		std::string prefix;

	public:
		/**
		 * Creates a new resource manager
		 *
		 * @param resourceLocator the resource locators
		 */
		ResourceManager(std::shared_ptr<Locator::ResourceLocator> resourceLocator, std::string prefix) :
				resourceLocator(resourceLocator), prefix(std::move(prefix)) {}

	public:
		/**
		 * Get a resource by its name
		 *
		 * @param resourceName the resource name
		 */
		virtual typename T::Ptr get(const std::string& resourceName) {
			std::cout << "Loading " << resourceName << std::endl;

			auto found = resources.find(resourceName);
			if(found != resources.end()) {
				return found->second;
			}

			auto resourceStream = resourceLocator->locate(prefix + resourceName);
			if(resourceStream == nullptr) {
				return nullptr;
			}

			for(std::unique_ptr<Loader>& loader : resourceLoaders) {
				if(!loader->supports(resourceStream)) {
					continue;
				}

				std::shared_ptr<T> resource = loader->load(std::move(resourceStream));
				if(resource == nullptr) {
					return nullptr;
				}

				resources.emplace(std::make_pair(
						resourceName, resource
				));
				return resource;
			}

			/*
			 * No loader could be found, returning nullptr
			 */
			return nullptr;
		}

		/**
		 * Add a new resource loader
		 *
		 * @param loader the resource loader to be added
		 */
		virtual void addResourceLoader(std::unique_ptr<Loader> resourceLoader) {
			resourceLoaders.push_back(std::move(resourceLoader));
		}

		/**
		 * @return the resource loader
		 */
		virtual std::vector<Loader*> getResourceLoaders() const {
			std::vector<Loader*> loaders;
			for(const std::unique_ptr<Loader>& loader : resourceLoaders) {
				loaders.push_back(loader.get());
			}
			return loaders;
		}

		/**
		 * Removes a resource loader
		 *
		 * @param resourceLoader the resource loader to be removed
		 *
		 * @return true if the loader was found and removed
		 */
		virtual bool removeResourceLoader(Loader* resourceLoader) {
			auto found = std::find_if(resourceLoaders.begin(), resourceLoaders.end(), [=](auto& entry) {
				return entry.get() == resourceLoader;
			});
			if(found == resourceLoaders.end()) {
				return false;
			}
			resourceLoaders.erase(found);
			return true;
		}

	protected:
		virtual void releaseResource(T* resource) {
//			auto found = std::find_if(resources.begin(), resources.end(), [resource](const auto& entry) {
//				return entry.second == resource;
//			});
//			if(found == resources.end()) {
//				return;
//			}
//			resources.erase(found);
		}

	};

}



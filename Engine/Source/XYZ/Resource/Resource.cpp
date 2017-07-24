//
// Created by Rogiel Sulzbach on 6/28/17.
//

#include "Resource.hpp"

namespace XYZ::Resource {

	AbstractResource::~AbstractResource() = default;

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractResource::canUnload() {
		return false;
	}

	void AbstractResource::unload() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractResource::isLoaded() {
		return true;
	}

	bool AbstractResource::canLoad() {
		return false;
	}

	void AbstractResource::load() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractResource::canReload() {
		return canUnload();
	}

	void AbstractResource::reload() {
		if(!canUnload()) {
			return;
		}
		unload();

		if(!canLoad()) {
			return;
		}
		load();
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractResource::didReceiveMemoryWarning() {
		return false;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Locator::ResourceLocator* AbstractResource::getResourceLocator() {
		return nullptr;
	}

	Locator::ResourceReference* AbstractResource::getResourceReference() {
		return nullptr;
	}

}
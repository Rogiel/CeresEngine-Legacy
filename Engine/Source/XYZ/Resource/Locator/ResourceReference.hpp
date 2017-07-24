//
// Created by Rogiel Sulzbach on 7/20/17.
//

#pragma once

namespace XYZ::Resource::Locator {

	/**
	 * A resource reference is a object that allows a resource locator
	 * to be able to relocate a resource.
	 *
	 * This is specially used when unloading resources so that a
	 * loader can request a data stream for the same resource
	 * that was loaded previously.
	 */
	class ResourceReference {

	};

}


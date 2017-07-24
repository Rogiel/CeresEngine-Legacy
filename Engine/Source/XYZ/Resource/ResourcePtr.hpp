//
// Created by Rogiel Sulzbach on 7/20/17.
//

#pragma once

#include <memory>

namespace XYZ::Resource {

	/**
	 * A shared resource pointer
	 */
	template<typename T>
	using ResourcePtr = std::shared_ptr<T>;

	/**
	 * A weak reference to a shared resource
	 */
	template<typename T>
	using WeakResourcePtr = std::weak_ptr<T>;

}



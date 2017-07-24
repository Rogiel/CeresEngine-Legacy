//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include <memory>
#include <XYZ/Resource/Locator/ResourceLocator.hpp>
#include <XYZ/Resource/Locator/ResourceReference.hpp>

#include "XYZ/Resource/ResourcePtr.hpp"

namespace XYZ::Resource {

	class AbstractResource {
	public:
		/**
		 * Virtual destructor.
		 *
		 * Resource specializations should override this method with
		 * a suitable cleanup that releases all the memory allocated
		 * for the resource.
		 */
		virtual ~AbstractResource();

	public:
		/**
		 * Checks if the resource can be unload (aka has at most 1 reference).
		 *
		 * Resource specializations might choose to override this implementation
		 * with a more suitable logic for their type.
		 *
		 * @return true if unload() can be called safely.
		 */
		virtual bool canUnload();

		/**
		 * Unloads the resource from memory.
		 */
		virtual void unload();

	public:
		/**
		 * Checks if the resource is already loaded.
		 *
		 * @return true if the resource is currently loaded
		 */
		virtual bool isLoaded();

		/**
		 * Checks if the resource can be loaded.
		 *
		 * Resource specializations might choose to override this implementation
		 * with a more suitable logic for their type.
		 *
		 * @return true if unload() can be called safely.
		 */
		virtual bool canLoad();

		/**
		 * Loads the resource from the locator pointer.
		 */
		virtual void load();

	public:
		/**
		 * Checks if the resource can be reloaded
		 *
		 * @return true if the resource can be reloaded.
		 */
		virtual bool canReload();

		/**
		 * Reloads the resource from the locator pointer.
		 *
		 * The default implementation calls unload() followed
		 * by a load().
		 */
		virtual void reload();

	public:
		/**
		 * A event called whenever the engine is running low on memory.
		 *
		 * When calling this method, resources are able to release
		 * cached data.
		 *
		 * @return true if the entire resource can be unloaded,
		 * false otherwise.
		 */
		virtual bool didReceiveMemoryWarning();

	public:
		/**
		 * @return the resource locator used to locate this resource, if any.
		 */
		virtual Locator::ResourceLocator* getResourceLocator();

		/**
		 * @return the resource reference returned by the resource locator
		 * when locating the resource. If the resource locator is null,
		 * the resource reference will most possibly be null too.
		 */
		virtual Locator::ResourceReference* getResourceReference();

	};

	/**
	 * The resource class is a base class inherited by all resource types.
	 *
	 * A resource is considered a piece of data that must be loaded
	 * either from a file, another resource or a string.
	 *
	 * Many resources are loaded by a <tt>ResourceLoader</tt> which takes
	 * a stream object returned by a <tt>ResourceLocator</tt>.
	 *
	 * The resource locator implements the parsing logic for a file type,
	 * while the resource locator is responsible for finding a physical
	 * representation for the resource.
	 *
	 * A resource locator can search for a resource inside a Mac OS X bundle,
	 * a ZIP file or even across the network.
	 *
	 * @tparam T the specialized resource type
	 */
	template<typename T>
	class Resource : public AbstractResource,
					 public std::enable_shared_from_this<T> {
	public:
		/**
		 * A reference counted pointer to the resource
		 */
		using Ptr = ResourcePtr<T>;

		/**
		 * A weak reference pointer to the resource
		 */
		using WeakPtr = WeakResourcePtr<T>;

	};

}



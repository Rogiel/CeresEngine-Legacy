//
// Created by Rogiel Sulzbach on 7/25/17.
//

#pragma once

#include <atomic>

namespace XYZ::WorldEditor::Utility {

	/**
	 * A copy-on-write wrapper for any model of a regular type.
	 *
	 * Copy-on-write semantics allow for object to be lazily copied - only creating a
	 * copy when the value is modified and there is more than one reference to the
	 * value.
	 *
	 * @tparam T the contained type
	 * @tparam A a optional allocator
	 */
	template<typename T, typename A = std::allocator<T>>
	class CopyOnWrite {
	public:
		/**
		 * A counter type to be used when reference counting
		 */
		using Counter = std::atomic<size_t>;

		/**
		 * The type of value stored.
		 */
		using Value = T;

		/**
		 * The type of allocator.
		 */
		using Allocator = A;

	private:
		struct Implementation;
		using ImplementationAllocator = typename Allocator::template rebind<Implementation>::other;

	public:
		/**
		 * The first call to the default constructor will construct a default
		 * instance of value_type which will be used for subsequent calls to the
		 * default constructor. The default instance will be released at exit.
		 */
		CopyOnWrite() {
			ImplementationAllocator allocator;
			object = allocate(allocator);
		}

		/**
		 * Initializes the instance with a custom allocator
		 */
		explicit CopyOnWrite(const Allocator& a) : object(nullptr) {
			ImplementationAllocator other_allocator(a);
			object = allocate(other_allocator);
		}

		/**
		 * @brief Constructs a new <tt>CopyOnWrite</tt> object with a value \c x.
		 * @param x A default value to assign to this object
		 */
		CopyOnWrite(T x) : object(allocateMove(nullptr, std::move(x))) {}

		/**
		 * Copy construction is a non-throwing operation and simply increments the
		 * reference count on the stored object.
		 */
		CopyOnWrite(const CopyOnWrite& x) : object(x.object) {
			if(object) {
				object->header.count++;
			}
		}

		CopyOnWrite(CopyOnWrite&& x) : object(x.object) {
			x.object = nullptr;
		}

		~CopyOnWrite() { release(object); }

		/**
		 * As with copy construction, assignment is a non-throwing operation which
		 * releases the old value and increments the reference count of the item
		 * being assigned to.
		 */
		CopyOnWrite& operator=(CopyOnWrite x) {
			swap(*this, x);
			return *this;
		}

		CopyOnWrite& operator=(T x) {
			if(!object)
				object = allocateMove(nullptr, std::move(x));
			else if(object->header.count == 1)
				object->value = std::move(x);
			else
				reset(allocateMove(object, std::move(x)));

			return *this;
		}

	public:
		/**
		 * Obtain a reference to the value the object is referencing. This will copy
		 * the underlying value (if necessary) so changes to the value do not affect
		 * other <tt>CopyOnWrite</tt> objects.
		 * Note that write() does not have the same preconditions as operator=().
		 * write() returns a reference to the underlying object's value, thus requiring
		 * that an underlying object exist. operator=() on the other hand will perform
		 * an allocation if one is necessary.
		 * @return A reference to the underlying object
		 */
		Value& write() {
			assert(object && "FATAL (sparent) : using a moved CopyOnWrite object");

			if(object->header.count != 1)
				reset(allocate(object, object->value));

			return object->value;
		}

		/**
		 * @brief Obtain a const reference to the underlying object.
		 * @return A const reference to the underlying object
		 */
		const Value& read() const {
			assert(object && "FATAL (sparent) : using a moved CopyOnWrite object");
			return object->value;
		}

		/**
		 * @brief Obtain a const reference to the underlying object.
		 * @return A const reference to the underlying object
		 */
		operator const Value&() const { return read(); }

		/**
		 * @brief Obtain a const reference to the underlying object.
		 *
		 * This is provided because you cannot override <code>operator.()</code>.
		 *
		 * @return A const reference to the underlying object
		 */
		const Value& operator*() const { return read(); }

		/**
		 * @brief Obtain a const pointer to the underlying object.
		 *
		 * This is provided because you cannot override <code>operator.()</code>.
		 *
		 * @return A const pointer to the underlying object
		*/
		const Value* operator->() const { return &read(); }

		/**
		 * @brief unique_instance returns whether or not the reference count to the
		 * object instance is one. This is useful to determine if writing will cause a
		 * copy.
		 *
		 * @return <code>true</code> if the ref count for the instance is one.
		*/
		bool unique() const { return !object || object->header.count == 1; }

		/**
		 * @brief identity is used to see if two <tt>CopyOnWrite</tt> items refer to the same
		 * instance.
		 *
		 * @return Boolean; <code>true</code> if the underlying object instance is
		 * shared by both objects.
		*/
		bool identity(const CopyOnWrite& x) const { return object == x.object; }

		friend inline void swap(CopyOnWrite& x, CopyOnWrite& y) {
			std::swap(x.object, y.object);
		}

		friend inline bool operator<(const CopyOnWrite& x, const CopyOnWrite& y) {
			return y.object && (!x.object || (!x.identity(y) && *x < *y));
		}

		friend inline bool operator>(const CopyOnWrite& x, const CopyOnWrite& y) { return y < x; }

		friend inline bool operator<=(const CopyOnWrite& x, const CopyOnWrite& y) {
			return !(y < x);
		}

		friend inline bool operator>=(const CopyOnWrite& x, const CopyOnWrite& y) {
			return !(x < y);
		}

		friend inline bool operator==(const CopyOnWrite& x, const CopyOnWrite& y) {
			return x.identity(y) || (x.object && y.object && *x == *y);
		}

		friend inline bool operator!=(const CopyOnWrite& x, const CopyOnWrite& y) {
			return !(x == y);
		}

		Allocator getAllocator() const {
			return object ? Allocator(object->get_allocator()) : Allocator();
		}

	private:
		static Implementation* allocate(const Implementation* alloc_src, const T& x = T()) {
			ImplementationAllocator allocator(alloc_src ? alloc_src->get_allocator()
														: ImplementationAllocator());

			return allocate(allocator, x);
		}

		static Implementation* allocate(ImplementationAllocator& allocator, const T& x = T()) {
			Implementation* tmp(allocator.allocate(1));

			try {
				::new(static_cast<void*>(tmp)) Implementation(x);
			}
			catch(...) {
				tmp->get_allocator().deallocate(tmp, 1);
				throw;
			}

			return tmp;
		}

		static Implementation* allocateMove(const Implementation* alloc_src, T x) {
			ImplementationAllocator allocator(alloc_src ? alloc_src->get_allocator()
														: ImplementationAllocator());
			Implementation* tmp(allocator.allocate(1));

			try {
				::new(static_cast<void*>(tmp)) Implementation(std::move(x));
			}
			catch(...) {
				tmp->get_allocator().deallocate(tmp, 1);
				throw;
			}

			return tmp;
		}

		static void release(Implementation* x) {
			if(!x || --x->header.count) return;

			ImplementationAllocator allocator(x->get_allocator());

			x->~Implementation();
			allocator.deallocate(x, 1);
		}

		void reset(Implementation* to) {
			release(object);
			object = to;
		}

		Implementation* object;
	};

	template<typename T, typename A>
	struct CopyOnWrite<T, A>::Implementation {
		// Assert proper size for Counter
		static_assert((sizeof(Counter) == sizeof(std::size_t)));
		// Assert proper alignment for Counter
		static_assert((sizeof(Counter) == sizeof(void*)));

		struct Header {
			Counter count;
			Allocator allocator;
		};

		explicit Implementation(T x) : value(std::move(x)) {
			header.count = 1;
		}

		ImplementationAllocator get_allocator() const {
			return ImplementationAllocator(header.allocator);
		}

		Header header;
		Value value;
	};

}


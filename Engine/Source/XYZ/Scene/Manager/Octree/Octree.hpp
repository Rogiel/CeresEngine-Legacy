//
// Created by Rogiel Sulzbach on 7/15/17.
//

#pragma once

#include <cassert>
#include <queue>

namespace XYZ::Scene::Manager::Octree {

	using OctreeSize = int;

	/**
	 * \class Octree
	 * \brief Generic octree template
	 *
	 * This class template represents an octree, often used for manipulating 3-D
	 * scattered data efficiently. The type of the contained data is supplied as a
	 * template parameter.
	 *
	 * \param T Type of the contained data. Requirements on type: must be copyable
	 * and default-constructible.
	 *
	 * \param I Type of the coordinate. Must be a integer type
	 *
	 * \param AS Short for "aggregate size." As an optimization, leaves can be
	 * aggregated so that the relative size of pointers is diminished. This is 1 by
	 * default, but should be set higher when the size of \a T is small. <b>Must be
	 * a power of two.</b>
	 *
	 * \param size Size of octree, in nodes. Should be a power of two. For example,
	 * an octree with \a size = 256 will represent a cube divided into 256x256x256
	 * nodes. <b>Must be a power of two.</b>
	 *
	 * \param emptyValue This is the value that will be returned when accessing
	 * regions of the 3-D volume where no node has been allocated. In other words,
	 * instead of following a null node pointer, this value is returned. Since the
	 * octree root is initially a null pointer, the whole volume is initialized to
	 * this value.
	 */
	template<typename T, OctreeSize S, typename I = int, int AS = 1>
	class Octree {
	public:
		class iterator;

		Octree(const T& emptyValue = T(0));

		/**
		 * Performs a deep copy of an octree. All branch pointers will be followed
		 * recursively and new nodes will be allocated.
		 *
		 * \param o octree to be copied.
		 */
		template<OctreeSize OS>
		Octree(const Octree<T, OS, I, AS>& o);

		/**
		 * Recursively deletes all nodes by following branch pointers.
		 */
		~Octree();

		// Accessors
		/**
		 * \return Size of octree, in nodes, as specified in the constructor.
		 */
		OctreeSize size() const;

		/**
		 * \return Value of empty nodes, as specified in the constructor.
		 * \see setEmptyValue()
		 */
		const T& emptyValue() const;

		/**
		 * \return Total number of nodes in the octree.
		 */
		int nodes() const;

		/**
		 * \return Number of nodes of at size \a size. For example, the root (if
		 * allocated) is the single node of size 1. At size <i>n</i> there may be a
		 * maximum of 2<sup><i>n</i></sup> nodes.
		 *
		 * For sizes lower than the aggregate size, this function will always return
		 * zero.
		 */
		int nodesAtSize(I size) const;

		// Mutators
		/**
		 * Sets the value of empty nodes to \a emptyValue.
		 * \see setEmptyValue()
		 */
		void setEmptyValue(const T& emptyValue);

		/**
		 * Swaps the octree's contents with another's. This is a cheap operation as only
		 * the root pointers are swapped, not the whole structure.
		 */
		template<OctreeSize OS>
		void swap(Octree<T, OS, I, AS>& o);

		/**
		 * Assigns to this octree the contents of octree \a o.
		 */
		template<OctreeSize OS>
		Octree<T, S, I, AS>& operator=(Octree<T, OS, I, AS> o);

		// Indexing operators
		/**
		 * \return Reference to value at index (\a x,\a y,\a z). If no node exists at
		 * this index, a new one is created (along with the necessary ancestry),
		 * initialized to the value returned by emptyValue(), and returned.
		 *
		 * \remarks Be careful when calling this function. If you do not want to
		 * inadvertently create new nodes, use the at() function.
		 *
		 * \see at()
		 */
		T& operator()(I x, I y, I z);

		/**
		 * Synonym of at().
		 */
		const T& operator()(I x, I y, I z) const;

		/**
		 * \return Value at index (\a x,\a y,\a z). If no node exists at this index, the
		 * value returned by emptyValue() is returned.
		 *
		 * \remarks Memory access is faster when \a x varies the quickest, followed by
		 * \a y and then by \a z. Therefore you should write nested loops in this order
		 * for faster access:
		 *
		 * \code
		 * for ( int z = 0; z < ...; ++z ) {
		 *     for ( int y = 0; y < ...; ++y ) {
		 *         for ( int x = 0; x < ...; ++x ) {
		 *             ... = octree.at(x,y,z);
		 *         }
		 *     }
		 * }
		 * \endcode
		 *
		 * However, zSlice() provides an even faster way.
		 */
		const T& at(I x, I y, I z) const;

		/**
		 * Sets the value of the node at (\a x, \a y, \a z) to \a value. If \a value is
		 * the empty value, the node is erased. Otherwise, the node is created if it did
		 * not already exist and its value is set to \a value.
		 */
		void set(I x, I y, I z, const T& value);

		/**
		 * Erases the node at index (\a x,\a y,\a z). After the call,
		 * <code>at(x,y,z)</code> will return the value returned by emptyValue().
		 *
		 * This function will free as much memory as possible. For example, when erasing
		 * the single child of a branch node, the branch node itself will be erased and
		 * replaced by a null pointer in its parent. This will percolate to the top of
		 * the tree if necessary.
		 */
		void erase(I x, I y, I z);

		int count();

		int countAproximate();

		template<OctreeSize OS>
		void merge(Octree<T, OS, I, AS>& other);

		/**
		 * Clears the entire tree
		 */
		void clear();

		/**
		 * Iterates over all elements of the tree
		 */
		void iterate(std::function<void(T&)> callback);

	protected:

		// octree node types
		class Node;

		class Branch;

		class Aggregate;

		class Leaf;

		enum NodeType {
			BranchNode, AggregateNode, LeafNode
		};

		/**
		 * \return Pointer to octree's root node.
		 */
		Node*& root();

		/**
		 * Const version of above.
		 */
		const Node* root() const;

		/**
		 * Deletes a node polymorphically. If the node is a branch node, it will delete
		 * all its subtree recursively.
		 */
		static void deleteNode(Node** node);

	public:
		iterator iterate();

		/**
		 * Allows iteration over each item in the tree
		 */
		class iterator : public std::iterator<std::forward_iterator_tag, T> {
			Octree<T, S, I, AS>* octree;

			T& value;

			struct node_entry {
				Node* node;
				int index = 0;
			};
			std::deque<node_entry*> nodes;

		public:
			iterator(Octree* octree);

			iterator(const iterator&);

			~iterator();

			iterator& operator=(const iterator&);

			iterator operator++();

			iterator operator++(int);

			friend void swap(const iterator& lhs, const iterator& rhs); //C++11 I think

			T& operator*() const;

			T operator->() const;

			friend bool operator==(const iterator&, const iterator&);

			friend bool operator!=(const iterator&, const iterator&);
		};

	private:
		// Recursive helper functions
		void iterateRecursive(Node* node, std::function<void(T&)> callback);

		/**
		 * Helper function for erase() method.
		 */
		void eraseRecursive(Node** node, I size, I x, I y, I z);

		/**
		 * Helper function for nodes() method.
		 */
		static int nodesRecursive(const Node* node);

		/**
		 * Helper function for nodesAtSize() method.
		 */
		static int nodesAtSizeRecursive(I targetSize, I size, Node* node);

	protected:
		// Node classes

		class Node {
		public:
			NodeType type() const;

		protected:
			Node(NodeType type);

			~Node() {};

		private:
			NodeType _type : 2;
		};

		class Branch : public Node {
			friend class Octree;

		public:
			Branch();

			Branch(const Branch& b);

			~Branch();

			const Node* child(I x, I y, I z) const;

			Node*& child(I x, I y, I z);

			const Node* child(I index) const;

			Node*& child(I index);

			friend void Octree<T, S, I, AS>::deleteNode(Node** node);

		private:
			Branch& operator=(Branch b);

		private:
			Node* children[2][2][2];
		};

		class Aggregate : public Node {
		public:
			Aggregate(const T& v);

			const T& value(I x, I y, I z) const;

			T& value(I x, I y, I z);

			void setValue(I x, I y, I z, const T& v);

			const T& value(I i) const;

			T& value(I i);

			void setValue(I i, const T& v);

			friend void Octree<T, S, I, AS>::deleteNode(Node** node);

		private:
			~Aggregate() {};

		private:
			T _value[AS][AS][AS];
		};

		class Leaf : public Node {
		public:
			Leaf(const T& v);

			const T& value() const;

			T& value();

			void setValue(const T& v);

			friend void Octree<T, S, I, AS>::deleteNode(Node** node);

		private:
			~Leaf() {};

		private:
			T _value;
		};

		static const OctreeSize _aggregateSize = AS;

	private:
		Node* _root;
		T _emptyValue;
	};

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Octree(const T& emptyValue)
			: _root(0), _emptyValue(emptyValue) {
		// Make sure size is power of two.
		static_assert(((S - 1) & S) == 0, "S must be a power of 2");
		static_assert(std::is_unsigned<I>::value, "I must be a unsigned type");
		static_assert(((AS - 1) & AS) == 0, "AS must be a power of 2");
	}

	template<typename T, OctreeSize S, typename I, int AS>
	template<OctreeSize OS>
	Octree<T, S, I, AS>::Octree(const Octree<T, OS, I, AS>& o)
			: _emptyValue(o._emptyValue) {
		static_assert(S >= OS, "S >= OS");

		if(!o._root) {
			_root = 0;
		} else {
			switch(o._root->type()) {
				case BranchNode:
					_root = new Branch(*reinterpret_cast<Branch*>(o._root));
					break;
				case LeafNode:
					_root = new Leaf(*reinterpret_cast<Leaf*>(o._root));
					break;
				case AggregateNode:
					_root = new Aggregate(*reinterpret_cast<Aggregate*>(o._root));
					break;
			}
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::~Octree() {
		deleteNode(&_root);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	template<OctreeSize OS>
	void Octree<T, S, I, AS>::swap(Octree<T, OS, I, AS>& o) {
		static_assert(S >= OS, "S >= OS");
		std::swap(_emptyValue, o._emptyValue);  // This can throw.

		// These can't.
		std::swap(_root, o._root);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	template<OctreeSize OS>
	Octree<T, S, I, AS>& Octree<T, S, I, AS>::operator=(Octree<T, OS, I, AS> o) {
		swap(o);
		return *this;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	OctreeSize Octree<T, S, I, AS>::size() const {
		return S;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::emptyValue() const {
		return _emptyValue;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::setEmptyValue(const T& emptyValue) {
		_emptyValue = emptyValue;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::deleteNode(Node** node) {
		assert(node);
		if(*node) {
			if((*node)->type() == BranchNode) {
				delete reinterpret_cast<Branch*>(*node);
			} else if((*node)->type() == AggregateNode) {
				delete reinterpret_cast<Aggregate*>(*node);
			} else {
				assert((*node)->type() == LeafNode);
				delete reinterpret_cast<Leaf*>(*node);
			}
			*node = 0;
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::Node*& Octree<T, S, I, AS>::root() {
		return _root;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const typename Octree<T, S, I, AS>::Node* Octree<T, S, I, AS>::root() const {
		return _root;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::at(I x, I y, I z) const {
		assert(x >= 0 && x < S);
		assert(y >= 0 && y < S);
		assert(z >= 0 && z < S);

		Node* n = _root;
		int size = S;

		while(size != _aggregateSize) {
			if(!n) {
				return _emptyValue;
			} else if(n->type() == BranchNode) {
				size /= 2;
				n = reinterpret_cast<Branch*>(n)->child(
						!!(x & size), !!(y & size), !!(z & size));
			} else {
				assert(n->type() == LeafNode);
				return reinterpret_cast<Leaf*>(n)->value();
			}
		}

		if(!n) {
			return _emptyValue;
		}

		--size;
		return reinterpret_cast<Aggregate*>(n)->value(
				x & size, y & size, z & size);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::operator()(I x, I y, I z) const {
		return at(x, y, z);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	T& Octree<T, S, I, AS>::operator()(I x, I y, I z) {
		assert(x >= 0 && x < S);
		assert(y >= 0 && y < S);
		assert(z >= 0 && z < S);

		Node** n = &_root;
		int size = S;

		while(size != _aggregateSize) {
			if(!*n) {
				*n = new Branch;
			} else if((*n)->type() == BranchNode) {
				size /= 2;
				n = &reinterpret_cast<Branch*>(*n)->child(
						!!(x & size), !!(y & size), !!(z & size));
			} else {
				return reinterpret_cast<Leaf*>(*n)->value();
			}
		}

		if(!*n) {
			*n = new Aggregate(_emptyValue);
		}

		--size;
		return reinterpret_cast<Aggregate*>(*n)->value(
				x & size, y & size, z & size);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::set(I x, I y, I z, const T& value) {
		if(value != emptyValue()) {
			(*this)(x, y, z) = value;
		} else {
			erase(x, y, z);
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::erase(I x, I y, I z) {
		assert(x >= 0 && x < S);
		assert(y >= 0 && y < S);
		assert(z >= 0 && z < S);

		eraseRecursive(&_root, S, x, y, z);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::count() {
		int count = 0;
		iterate([&count](T obj) {
			count++;
		});
		return count;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::countAproximate() {
		return (nodes() / 2) * AS * AS * AS;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::clear() {
		deleteNode(&_root);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::iterate(std::function<void(T&)> callback) {
		if(!_root)
			return;

		iterateRecursive(_root, callback);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::iterateRecursive(Node* node, std::function<void(T&)> callback) {
		if(node->type() == BranchNode) {
			for(int i = 0; i < 8; ++i) {
				Branch* b = reinterpret_cast<Branch*>(node);
				if(b->child(i)) {
					iterateRecursive(b->child(i), callback);
				}
			}
		} else if(node->type() == AggregateNode) {
			Aggregate* aggregate = reinterpret_cast<Aggregate*>(node);

			for(int i = 0; i < AS * AS * AS; i++) {
				if(aggregate->value(i) != _emptyValue) {
					callback(aggregate->value(i));
				}
			}
		} else {
			Leaf* leaf = reinterpret_cast<Leaf*>(node);
			if(leaf->value() != _emptyValue) {
				callback(leaf->value());
			}
		}
	}


	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::iterator Octree<T, S, I, AS>::iterate() {
		return iterator(this);
	};

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::iterator::iterator(Octree* octree) : octree(octree), value(octree->_emptyValue) {
		node_entry entry;
		entry.node = octree->root();
		nodes.push_front(&entry);
		operator++();
	};

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::iterator::iterator(const iterator& other) : value(other.octree->_emptyValue) {
		//current_node = other.current_node;
		octree = other.octree;
		//child_index = other.child_index;
		//branch_queue = other.branch_queue;
	};

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::iterator::~iterator() {

	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::iterator&
	Octree<T, S, I, AS>::iterator::operator=(const typename Octree::iterator& other) {
		//current_node = other.current_node;
		octree = other.octree;
		//child_index = other.child_index;
		//branch_queue = other.branch_queue;
		return *this;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::iterator Octree<T, S, I, AS>::iterator::operator++() {
		if(nodes.empty()) {
			value = octree->_emptyValue;
			return *this;
		}

		while(!nodes.empty()) {
			node_entry entry = *nodes.back();
			nodes.pop_back();
			Node* node = entry.node;

			if(node->type() == BranchNode) {
				Branch* b = reinterpret_cast<Branch*>(node);
				for(; entry.index < 8; entry.index++) { // first non-empty
					if(b->child(entry.index)) {
						node_entry new_entry;
						new_entry.node = b->child(entry.index);
						if(entry.index < 8) {
							nodes.push_back(&entry);
						}
						nodes.push_back(&new_entry);
						break;
					}
				}
			} else if(node->type() == AggregateNode) {
				bool breakIt = false;

				Aggregate* aggregate = reinterpret_cast<Aggregate*>(node);
				for(; entry.index < AS * AS * AS; entry.index++) {
					if(aggregate->value(entry.index) != octree->_emptyValue) {
						value = aggregate->value(entry.index);
						breakIt = true;

						if(entry.index != (AS * AS * AS - 1)) {
							nodes.push_back(&entry);
						}

						break;
					}
				}
				if(breakIt) break;
			} else {
				Leaf* leaf = reinterpret_cast<Leaf*>(node);
				if(leaf->value() != octree->_emptyValue) {
					value = leaf->value();
					break;
				}
			}
		}
		return *this;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::iterator Octree<T, S, I, AS>::iterator::operator++(int n) {
		for(int i = 0; i < n; i++) {
			operator++();
		}
		return *this;
	}


	template<typename T, OctreeSize S, typename I, int AS>
	T& Octree<T, S, I, AS>::iterator::operator*() const {
//        if(current_node->type() == AggregateNode) {
//            return reinterpret_cast<Aggregate*>(current_node)->value(aggregator_index);
//        } else {
//            return reinterpret_cast<Leaf*>(current_node)->value();
//        }
		return value;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	T Octree<T, S, I, AS>::iterator::operator->() const {
		return value;
	}


	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::eraseRecursive(Node** node, I size, I x, I y, I z) {
		assert(node);

		if(!*node) {
			return;
		}

		if(size != _aggregateSize) {
			if((*node)->type() == BranchNode) {
				size /= 2;
				Branch* b = reinterpret_cast<Branch*>(*node);
				eraseRecursive(&b->child(!!(x & size), !!(y & size), !!(z & size)),
							   size, x, y, z);

				for(int i = 0; i < 8; ++i) {
					if(b->child(i)) {
						return;
					}
				}
				deleteNode(node);
			} else if(reinterpret_cast<Leaf*>(*node)->value() == _emptyValue) {
				deleteNode(node);
			} else {
				Branch* b = new Branch;
				size /= 2;
				int childIndex = (x & size ? 1 : 0)
								 | (y & size ? 2 : 0)
								 | (z & size ? 4 : 0);
				const T& value = reinterpret_cast<Leaf*>(*node)->value();
				try {
					for(int i = 0; i < 8; ++i) {
						if(i == childIndex) {
							continue;
						}
						if(size == _aggregateSize) {
							b->child(i) = new Leaf(value);
						} else {
							b->child(i) = new Aggregate(value);
						}
					}
				}
				catch(...) {
					Node* bb = b;
					deleteNode(&bb);
					throw;
				}

				deleteNode(node);
				*node = b;
				node = &b->child(childIndex);
			}
		} else {
			--size;
			Aggregate* a = reinterpret_cast<Aggregate*>(*node);
			a->setValue(x & size, y & size, z & size, _emptyValue);

			for(int i = 0; i < AS * AS * AS; ++i) {
				if(a->value(i) != _emptyValue) {
					return;
				}
			}
			deleteNode(node);
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::nodes() const {
		return nodesRecursive(_root);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::nodesRecursive(const Node* node) {
		if(!node) {
			return 0;
		}

		int n = 1;
		if(node->type() == BranchNode) {
			for(int i = 0; i < 8; ++i) {
				n += nodesRecursive(reinterpret_cast<const Branch*>(node)->child(i));
			}
		}

		return n;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::nodesAtSize(I size) const {
		return nodesAtSizeRecursive(size, S, _root);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	int Octree<T, S, I, AS>::nodesAtSizeRecursive(I targetSize, I size, Node* node) {
		if(node) {
			if(size == targetSize) {
				return 1;
			}

			if(node->type() == BranchNode) {
				int sum = 0;
				for(int i = 0; i < 2; ++i) {
					for(int j = 0; j < 2; ++j) {
						for(int k = 0; k < 2; ++k) {
							sum += nodesAtSizeRecursive(targetSize, size / 2,
														reinterpret_cast<Branch*>(node)->child(k, j, i));
						}
					}
				}
				return sum;
			}
		}

		return 0;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Node::Node(NodeType type)
			: _type(type) {
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::NodeType Octree<T, S, I, AS>::Node::type() const {
		return _type;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Branch::Branch()
			: Node(BranchNode) {
		memset(children, 0, sizeof(children));
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Branch::Branch(const Branch& b)
			: Node(BranchNode) {
		for(int i = 0; i < 8; ++i) {
			if(b.child(i)) {
				switch(b.child(i)->type()) {
					case BranchNode:
						child(i) = new Branch(
								*reinterpret_cast<const Branch*>(b.child(i)));
						break;
					case LeafNode:
						child(i) = new Leaf(
								*reinterpret_cast<const Leaf*>(b.child(i)));
						break;
					case AggregateNode:
						child(i) = new Aggregate(
								*reinterpret_cast<const Aggregate*>(b.child(i)));
						break;
				}
			} else {
				child(i) = 0;
			}
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Branch::~Branch() {
		for(int i = 0; i < 2; ++i) {
			for(int j = 0; j < 2; ++j) {
				for(int k = 0; k < 2; ++k) {
					assert(children[i][j][k] != this);
					deleteNode(&children[i][j][k]);
				}
			}
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const typename Octree<T, S, I, AS>::Node* Octree<T, S, I, AS>::Branch::child(I x, I y, I z) const {
		assert(x == 0 || x == 1);
		assert(y == 0 || y == 1);
		assert(z == 0 || z == 1);
		return children[z][y][x];
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::Node*& Octree<T, S, I, AS>::Branch::child(I x, I y, I z) {
		assert(x == 0 || x == 1);
		assert(y == 0 || y == 1);
		assert(z == 0 || z == 1);
		return children[z][y][x];
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const typename Octree<T, S, I, AS>::Node* Octree<T, S, I, AS>::Branch::child(I index)
	const {
		assert(index >= 0 && index < 8);
		return *(&children[0][0][0] + index);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	typename Octree<T, S, I, AS>::Node*& Octree<T, S, I, AS>::Branch::child(I index) {
		assert(index >= 0 && index < 8);
		return *(&children[0][0][0] + index);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Aggregate::Aggregate(const T& v)
			: Node(AggregateNode) {
		for(int i = 0; i < AS; ++i) {
			for(int j = 0; j < AS; ++j) {
				for(int k = 0; k < AS; ++k) {
					_value[i][j][k] = v;
				}
			}
		}
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::Aggregate::value(I x, I y, I z) const {
		assert(x >= 0 && x < AS);
		assert(y >= 0 && y < AS);
		assert(z >= 0 && z < AS);

		return _value[z][y][x];
	}

	template<typename T, OctreeSize S, typename I, int AS>
	T& Octree<T, S, I, AS>::Aggregate::value(I x, I y, I z) {
		assert(x >= 0 && x < AS);
		assert(y >= 0 && y < AS);
		assert(z >= 0 && z < AS);

		return _value[z][y][x];
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::Aggregate::setValue(I x, I y, I z, const T& v) {
		assert(x >= 0 && x < AS);
		assert(y >= 0 && y < AS);
		assert(z >= 0 && z < AS);

		_value[z][y][x] = v;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::Aggregate::value(I i) const {
		assert(i >= 0 && i < AS * AS * AS);

		return *(&_value[0][0][0] + i);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	T& Octree<T, S, I, AS>::Aggregate::value(I i) {
		assert(i >= 0 && i < AS * AS * AS);

		return *(&_value[0][0][0] + i);
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::Aggregate::setValue(I i, const T& v) {
		assert(i >= 0 && i < AS * AS * AS);

		*(&_value[0][0][0] + i) = v;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	Octree<T, S, I, AS>::Leaf::Leaf(const T& v)
			: Node(LeafNode), _value(v) {
	}

	template<typename T, OctreeSize S, typename I, int AS>
	const T& Octree<T, S, I, AS>::Leaf::value() const {
		return _value;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	T& Octree<T, S, I, AS>::Leaf::value() {
		return _value;
	}

	template<typename T, OctreeSize S, typename I, int AS>
	void Octree<T, S, I, AS>::Leaf::setValue(const T& v) {
		_value = v;
	}

}






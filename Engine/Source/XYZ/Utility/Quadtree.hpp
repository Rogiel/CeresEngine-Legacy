//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace XYZ::Utility {

	template<class T, size_t BucketCapacity = 4>
	class Quadtree {
	public:
		using Point = glm::vec2;

		using Node = std::pair<Point, T>;

		struct AABB {
			Point centre;
			Point halfSize;

			AABB(Point centre = Point(), Point halfSize = Point()) : centre(centre), halfSize(halfSize) {};

			bool contains(const Point& a) const {
				if(a.x < centre.x + halfSize.x && a.x > centre.x - halfSize.x) {
					if(a.y < centre.y + halfSize.y && a.y > centre.y - halfSize.y) {
						return true;
					}
				}
				return false;
			}

			bool intersects(const AABB& other) const {
				if(centre.x + halfSize.x > other.centre.x - other.halfSize.x ||
				   centre.x - halfSize.x < other.centre.x + other.halfSize.x) {
					if(centre.y + halfSize.y > other.centre.y - other.halfSize.y ||
					   centre.y - halfSize.y < other.centre.y + other.halfSize.y) {
						return true;
					}
				}
				return false;
			}
		};

	private:
		AABB boundary;
		Quadtree* children[4] = {
				nullptr, nullptr, nullptr, nullptr
		};

		std::vector<Node> objects;

	public:
		Quadtree() {
			Quadtree::objects.reserve(BucketCapacity);
		}

		explicit Quadtree(AABB boundary) : boundary(boundary) {
			Quadtree::objects.reserve(BucketCapacity);
		}

		~Quadtree() {
			for(Quadtree* child : children) {
				delete child;
			}
		}

		bool insert(Node node) {
			if(!boundary.contains(node.first)) {
				return false;
			}

			if(objects.size() < objects.capacity()) {
				objects.push_back(node);
				return true;
			}

			if(children[0] == nullptr) {
				subdivide();
			}

			for(Quadtree* child : children) {
				if(child->fits(node.first)) {
					return child->insert(std::move(node));
				}
			}

			return false;
		}

		bool insert(Point p, T object) {
			return insert(std::make_pair(std::move(p), std::move(object)));
		}

		bool fits(const Point& point) {
			return boundary.contains(point);
		}

		void subdivide() {
			Point qSize = Point(boundary.halfSize.x, boundary.halfSize.y);
			Point qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y - qSize.y);

			children[0] = new Quadtree(AABB(qCentre, qSize));

			qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y - qSize.y);
			children[1] = new Quadtree(AABB(qCentre, qSize));

			qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y + qSize.y);
			children[2] = new Quadtree(AABB(qCentre, qSize));

			qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y + qSize.y);
			children[3] = new Quadtree(AABB(qCentre, qSize));
		}

		using QueryList = std::vector<const Node*>;

		QueryList queryRange(const AABB& range) const {
			QueryList pInRange;

			if(!boundary.intersects(range)) {
				return pInRange;
			}

			for(int i = 0; i < objects.size(); i++) {
				if(range.contains(objects.at(i).first)) {
					pInRange.push_back(&objects.at(i));
				}
			}

			if(children[0] == nullptr) {
				return pInRange;
			}

			for(Quadtree* child : children) {
				QueryList temp = child->queryRange(range);
				pInRange.insert(pInRange.end(), temp.begin(), temp.end());
			}

			return pInRange;
		}

		const Node& operator()(float x, float y) const {
			return (*this)(Point(x, y));
		}

		const Node& operator()(const Point& point) const {
			for(int i = 0; i < objects.size(); i++) {
				if(objects.at(i).first.x == point.x && objects.at(i).first.y == point.y) {
					return objects.at(i);
				}
			}

			if(children[0] == nullptr) {
				throw std::runtime_error("Object not found!");
			}

			for(Quadtree* child : children) {
				if(child->fits(point)) {
					return (*child)(point);
				}
			}

			throw std::runtime_error("Object not found!");
		}
	};

}

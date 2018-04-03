//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include "XYZ/Terrain/Manager/Quadtree/QuadtreeTerrainPatch.hpp"
#include "XYZ/Utility/Quadtree.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace XYZ::Terrain::Manager::Quadtree {

	/**
	 * The TerrainQuadtree is a structure that structures the terrain patches
	 * in a way that can be easily traversed for a list of visible patches.
	 *
	 * The Quadtree is also responsible for "packing" far patches into a single
	 * larger patch.
	 */
	class TerrainQuadtree {
	public:
		using Point = glm::vec2;

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

				return (fabsf(centre.x - other.centre.x) < (halfSize.x + other.halfSize.x)) &&
						(fabsf(centre.y - other.centre.y) < (halfSize.y + other.halfSize.y));

//				std::cout << " --- " << std::endl;
//				std::cout << "x: " << centre.x + halfSize.x << " > " << other.centre.x - other.halfSize.x << std::endl;
//				std::cout << "x: " << centre.x - halfSize.x << " < " << other.centre.x + other.halfSize.x << std::endl;
//
//				if(centre.x + halfSize.x > other.centre.x - other.halfSize.x ||
//				   centre.x - halfSize.x < other.centre.x + other.halfSize.x) {
//					std::cout << "y: " << centre.y + halfSize.y << " > " << other.centre.y - other.halfSize.y << std::endl;
//					std::cout << "y: " << centre.y - halfSize.y << " < " << other.centre.y + other.halfSize.y << std::endl;
//
//
//					if(centre.y + halfSize.y > other.centre.y - other.halfSize.y ||
//					   centre.y - halfSize.y < other.centre.y + other.halfSize.y) {
//						return true;
//					}
//				}
				return false;
			}
		};

	private:
		AABB boundary;
		std::unique_ptr<TerrainQuadtree> children[4];
		int lod = 0;

		static const uint8_t C = 8;

	public:
		explicit TerrainQuadtree(AABB boundary);
		TerrainQuadtree(int lod, AABB boundary);

		~TerrainQuadtree();

		float f_(float d, float d2, float l);
		void update(glm::vec3 cameraPosition);
		void subdivide();

		using QueryList = std::vector<std::pair<AABB, int>>;

		QueryList queryRange(const AABB& range) const;

	private:
		AABB doQueryRange(const AABB& range) const;

	public:
		std::string toSVG();
		std::string toSVG(const glm::vec2& offset);

	};

}


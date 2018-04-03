//
// Created by Rogiel Sulzbach on 8/12/17.
//

#include "OctreeTerrainPatch.hpp"

namespace XYZ::Terrain::Manager::Octree {

	OctreeTerrainPatch::OctreeTerrainPatch() = default;
	OctreeTerrainPatch::OctreeTerrainPatch(glm::vec2 center, glm::vec2 size) :
			center(center), size(size) {}
	OctreeTerrainPatch::~OctreeTerrainPatch() = default;

	// -----------------------------------------------------------------------------------------------------------------

	glm::vec2 OctreeTerrainPatch::getCenter() const {
		return center;
	}

	glm::vec2 OctreeTerrainPatch::getSize() const {
		return size;
	}

}
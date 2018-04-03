//
// Created by Rogiel Sulzbach on 8/12/17.
//

#include "QuadtreeTerrainPatch.hpp"

namespace XYZ::Terrain::Manager::Quadtree {

	QuadtreeTerrainPatch::QuadtreeTerrainPatch() = default;
	QuadtreeTerrainPatch::QuadtreeTerrainPatch(glm::vec2 center, glm::vec2 size, Terrain::LevelOfDetail levelOfDetail) :
			center(center), size(size), levelOfDetail(levelOfDetail) {}
	QuadtreeTerrainPatch::~QuadtreeTerrainPatch() = default;

	// -----------------------------------------------------------------------------------------------------------------

	glm::vec2 QuadtreeTerrainPatch::getCenter() const {
		return center;
	}

	glm::vec2 QuadtreeTerrainPatch::getSize() const {
		return size;
	}

	unsigned int QuadtreeTerrainPatch::getLevelOfDetail() const {
		return levelOfDetail;
	}

}
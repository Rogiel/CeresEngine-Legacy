//
// Created by Rogiel Sulzbach on 8/12/17.
//

#include "TerrainQuadtree.hpp"

#include <cmath>
#include <sstream>
#include <iostream>

namespace XYZ::Terrain::Manager::Quadtree {

	TerrainQuadtree::TerrainQuadtree(AABB boundary) : TerrainQuadtree(0, boundary) {}
	TerrainQuadtree::TerrainQuadtree(int lod, AABB boundary) : boundary(boundary), lod(lod) {}

	TerrainQuadtree::~TerrainQuadtree() = default;

	// -----------------------------------------------------------------------------------------------------------------

	float TerrainQuadtree::f_(float d, float d2, float l) {
		return (l / d) / float(C);
	}

	void TerrainQuadtree::update(glm::vec3 cameraPosition) {
		if(lod == C) {
			return;
		}

//			float l = glm::length(glm::distance(cameraPosition, glm::vec3(boundary.centre.x, 0.0, boundary.centre.y)));

		// use a simplified distance model
		float l = std::fabsf(cameraPosition.x - boundary.centre.x) + std::fabsf(cameraPosition.y - 0.0f) +
				  std::fabsf(cameraPosition.z - boundary.centre.y);

		float f = f_(boundary.halfSize.x, 0.0f, l);

		if(f < 1.0) {
			subdivide();
			for(const std::unique_ptr<TerrainQuadtree>& child : children) {
				child->update(cameraPosition);
			}
		}
	}

	void TerrainQuadtree::subdivide() {
		{
			auto center = boundary.centre - boundary.halfSize / 2.0f;
			children[0] = std::make_unique<TerrainQuadtree>(lod + 1, AABB(center, boundary.halfSize / 2.0f));
		}

		{
			auto center = boundary.centre;
			center.x += boundary.halfSize.x / 2.0f;
			center.y -= boundary.halfSize.y / 2.0f;
			children[1] = std::make_unique<TerrainQuadtree>(lod + 1, AABB(center, boundary.halfSize / 2.0f));
		}

		{
			auto center = boundary.centre;
			center.x -= boundary.halfSize.x / 2.0f;
			center.y += boundary.halfSize.y / 2.0f;
			children[2] = std::make_unique<TerrainQuadtree>(lod + 1, AABB(center, boundary.halfSize / 2.0f));
		}

		{
			auto center = boundary.centre + boundary.halfSize / 2.0f;
			children[3] = std::make_unique<TerrainQuadtree>(lod + 1, AABB(center, boundary.halfSize / 2.0f));
		}
	}

	TerrainQuadtree::QueryList TerrainQuadtree::queryRange(const TerrainQuadtree::AABB& range) const  {
		QueryList pInRange;
		if(!boundary.intersects(range)) {
			return pInRange;
		}

		if(children[0] == nullptr) {
			pInRange.push_back(std::make_pair(boundary, lod));
			return pInRange;
		}

		for(const std::unique_ptr<TerrainQuadtree>& child : children) {
			QueryList temp = child->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());
		}

		return pInRange;
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::string TerrainQuadtree::toSVG() {
		return toSVG(boundary.halfSize);
	}

	std::string TerrainQuadtree::toSVG(const glm::vec2& offset) {
		std::stringstream os;

		auto xy = boundary.centre - boundary.halfSize + offset;
		auto wh = boundary.halfSize * 2.0f;

		if(children[0] != nullptr) {
			for(const std::unique_ptr<TerrainQuadtree>& child : children) {
				os << child->toSVG(offset);
			}
		} else {
			os << "<rect x=\"" << xy.x << "\" y=\"" << xy.y
			   << "\" width=\"" << wh.x << "\" height=\"" << wh.y
			   << "\" fill=\"rgb(0%, " << (int) (float(lod) / float(C) * 100) <<"%, 0%)\" stroke-width=\"0.5\" stroke=\"black\" fill-opacity=\"1.0\" />" << std::endl;
		}

		return os.str();
	}

}
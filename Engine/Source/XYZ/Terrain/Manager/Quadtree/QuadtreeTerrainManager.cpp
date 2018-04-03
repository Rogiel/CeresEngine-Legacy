//
// Created by Rogiel Sulzbach on 8/12/17.
//

#include "QuadtreeTerrainManager.hpp"
#include "TerrainQuadtree.hpp"

#include <glm/ext.hpp>
#include <iostream>

namespace XYZ::Terrain::Manager::Quadtree {

	QuadtreeTerrainManager::QuadtreeTerrainManager(Terrain::Ptr terrain, std::shared_ptr<Scene::Camera> camera) :
			terrain(terrain), camera(camera) {}

	QuadtreeTerrainManager::~QuadtreeTerrainManager() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	TerrainManager::TerrainPatchVector QuadtreeTerrainManager::getVisibleTerrainPatches() {
		auto positionWithZoom = camera->getPosition() - camera->Front * camera->Zoom;
		auto projection = glm::perspective(camera->getFieldOfView(), 4.0f / 3.0f, camera->getZNear(),
										   camera->getZFar());
		auto view = glm::lookAt(positionWithZoom, positionWithZoom + camera->getFront(), camera->getUp());
		auto transform = projection * view;
		auto inverseTransform = inverse(transform);

		glm::vec3 bounds[2][2][2];
		glm::vec3 max, min;

		for(int i = -1; i <= 1; i = i + 2) {
			for(int j = -1; j <= 1; j = j + 2) {
				for(int k = -1; k <= 1; k = k + 2) {
					auto inversed = inverseTransform * glm::vec4(glm::vec3(i, j, k), 1.0);
					bounds[(i + 1) / 2][(j + 1) / 2][(k + 1) / 2] = glm::vec3(inversed / inversed.w);

					max.x = std::max(max.x, bounds[i][j][k].x);
					max.y = std::max(max.y, bounds[i][j][k].y);
					max.z = std::max(max.z, bounds[i][j][k].z);

					min.x = std::min(min.x, bounds[i][j][k].x);
					min.y = std::min(min.y, bounds[i][j][k].y);
					min.z = std::min(min.z, bounds[i][j][k].z);
				}
			}
		}

		/*
		 * Compute the Quadtree size.
		 *
		 * We round to a multiple of a known number so that previous
		 * results can be cached (i.e. the quadtree will automatically
		 * reuse previous data).
		 */
		float size = glm::ceilMultiple(std::max(
				std::max(
						std::max(bounds[0][0][1].x, bounds[0][0][1].z),
						std::max(bounds[1][0][1].x, bounds[1][0][1].z)
				),
				std::max(
						std::max(bounds[1][1][1].x, bounds[1][1][1].z),
						std::max(bounds[0][1][1].x, bounds[0][1][1].z)
				)
		), 50.0f);
		TerrainQuadtree quadtree(
				0, {{0,    0},
					{size, size}}
		);
		quadtree.update(camera->getPosition());

		glm::vec2 boxMax = glm::vec2(max.x, max.z);
		glm::vec2 boxMin = glm::vec2(min.x, min.z);

		glm::vec2 centre = (boxMax + boxMin) / 2.0f;
		glm::vec2 halfSize = (boxMax - boxMin) / 2.0f;

		TerrainPatchVector patches;
		for(auto& patch : quadtree.queryRange({centre, halfSize})) {
			patches.push_back(std::make_shared<QuadtreeTerrainPatch>(patch.first.centre, patch.first.halfSize * 2.0f, patch.second));
		}

		return patches;
	}

	Terrain& QuadtreeTerrainManager::getTerrain() {
		return *terrain;
	}

	void QuadtreeTerrainManager::setTerrain(Terrain& terrain) {
		QuadtreeTerrainManager::terrain = terrain.shared_from_this();
	}

}
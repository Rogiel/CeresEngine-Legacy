//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include "XYZ/Terrain/Manager/TerrainPatch.hpp"

#include <glm/vec2.hpp>

namespace XYZ::Terrain::Manager::Octree {

	class OctreeTerrainPatch : public TerrainPatch {
	private:
		/**
		 * The terrain patch center coordinate (in world space)
		 */
		glm::vec2 center;

		/**
		 * The terrain patch size (in world space)
		 */
		glm::vec2 size;

	public:
		/**
		 * Creates a new empty terrain patch
		 */
		OctreeTerrainPatch();

		/**
		 * Creates a new terrain patch
		 *
		 * @param center the terrain patch center coordinate (in world space)
		 * @param size the terrain patch size (in world space)
		 */
		OctreeTerrainPatch(glm::vec2 center, glm::vec2 size);

		/**
		 * Destroys the terrain patch.
		 */
		~OctreeTerrainPatch();

	public:


	public:
		/**
		 * @return the terrain patch center coordinate (in world space)
		 */
		glm::vec2 getCenter() const final override;

		/**
		 * @return the terrain patch size (in world space)
		 */
		glm::vec2 getSize() const final override;
	};

}

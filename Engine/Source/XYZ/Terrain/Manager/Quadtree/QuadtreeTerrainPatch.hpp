//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include "XYZ/Terrain/Manager/TerrainPatch.hpp"
#include "XYZ/Terrain/Terrain.hpp"

#include <glm/vec2.hpp>

namespace XYZ::Terrain::Manager::Quadtree {

	class QuadtreeTerrainPatch : public TerrainPatch {
	private:
		/**
		 * The terrain patch center coordinate (in world space)
		 */
		glm::vec2 center;

		/**
		 * The terrain patch size (in world space)
		 */
		glm::vec2 size;

		/**
		 * The terrain level of detail
		 */
		Terrain::LevelOfDetail levelOfDetail;

	public:
		/**
		 * Creates a new empty terrain patch
		 */
		QuadtreeTerrainPatch();

		/**
		 * Creates a new terrain patch
		 *
		 * @param center the terrain patch center coordinate (in world space)
		 * @param size the terrain patch size (in world space)
		 */
		QuadtreeTerrainPatch(glm::vec2 center, glm::vec2 size, Terrain::LevelOfDetail levelOfDetail);

		/**
		 * Destroys the terrain patch.
		 */
		~QuadtreeTerrainPatch();

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

		unsigned int getLevelOfDetail() const final;
	};

}

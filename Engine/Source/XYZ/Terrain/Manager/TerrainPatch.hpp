//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include <glm/vec2.hpp>

namespace XYZ::Terrain::Manager {

	class TerrainPatch {
	public:
		virtual ~TerrainPatch();

	public:
		/**
		 * @return the terrain patch center coordinate (in world space)
		 */
		virtual glm::vec2 getCenter() const = 0;

		/**
		 * @return the terrain patch size (in world space)
		 */
		virtual glm::vec2 getSize() const = 0;

		virtual unsigned int getLevelOfDetail() const = 0;

	};

}

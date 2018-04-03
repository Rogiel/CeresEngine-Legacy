//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include "XYZ/Terrain/Terrain.hpp"
#include "XYZ/Terrain/Manager/TerrainPatch.hpp"

#include <vector>

namespace XYZ::Terrain::Manager {

	/**
	 * The Terrain Manager is a manager class that is responsible for organizing
	 * and managing terrain patches. Each patch is a squared piece of terrain that
	 * can be rendered into a scene.
	 *
	 * Each patch can be queried for visibility. Once a patch is not visible
	 * and from a certain distance from the camera, the manager can automatically
	 * unload the patch.
	 *
	 * In the same way as the manager does unloading, the manager can also
	 * merge multiple small patches that are very far away from the camera
	 * into a larger patch. This merging helps keeping the number of draw
	 * calls low.
	 */
	class TerrainManager {
	public:
		/**
		 * A vector of terrain patches pointers
		 */
		using TerrainPatchVector = std::vector<std::shared_ptr<TerrainPatch>>;

	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~TerrainManager();

	public:
		/**
		 * Gets a list of visible terrain patches.
		 *
		 * @return a list of visible terrain patches
		 */
		virtual TerrainPatchVector getVisibleTerrainPatches() = 0;

	public:
		/**
		 * @return the terrain currently being managed by
		 * this manager.
		 */
		virtual Terrain& getTerrain() = 0;

		/**
		 * Changes the terrain the TerrainManager is managing.
		 *
		 * @param terrain the new terrain to be handled by the manager
		 */
		virtual void setTerrain(Terrain& terrain) = 0;

	};

}

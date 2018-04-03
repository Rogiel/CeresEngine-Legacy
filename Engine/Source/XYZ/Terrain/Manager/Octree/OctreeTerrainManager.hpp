//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#import "XYZ/Terrain/Manager/TerrainManager.hpp"

namespace XYZ::Terrain::Manager::Octree {

	/**
	 * The Octree Terrain Manager is a terrain manager implementation
	 * that uses a Octree structure to organize the terrain into smaller
	 * terrain patches.
	 *
	 * Each patch can be queried for visibility. Once a patch is not visible
	 * and from a certain distance from the camera, the manager will
	 * automatically unload the patch.
	 *
	 * In the same way as the manager does unloading, the manager will also
	 * merge multiple small patches that are very far away from the camera
	 * into a larger patch. This merging helps keeping the number of draw
	 * calls low.
	 */
	class OctreeTerrainManager : public TerrainManager {
	public:
		/**
		 * Creates a new Octree Terrain Manager
		 */
		OctreeTerrainManager();

		/**
		 * Destroys tge Octree Terrain Manager
		 */
		~OctreeTerrainManager();

	public:


	};

}


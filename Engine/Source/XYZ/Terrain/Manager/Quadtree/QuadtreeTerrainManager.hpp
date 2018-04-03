//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#import "XYZ/Terrain/Manager/TerrainManager.hpp"
#include "XYZ/Scene/Camera.hpp"

namespace XYZ::Terrain::Manager::Quadtree {

	/**
	 * The Quadtree Terrain Manager is a terrain manager implementation
	 * that uses a quadtree structure to organize the terrain into smaller
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
	class QuadtreeTerrainManager : public TerrainManager {
	private:
		/**
		 * The terrain being managed by this manager
		 */
		Terrain::Ptr terrain;

		/**
		 * The camera viewing the terrain
		 */
		std::shared_ptr<Scene::Camera> camera;

	public:
		/**
		 * Creates a new Quadtree Terrain Manager
		 */
		QuadtreeTerrainManager(Terrain::Ptr terrain, std::shared_ptr<Scene::Camera> camera);

		/**
		 * Destroys tge Quadtree Terrain Manager
		 */
		~QuadtreeTerrainManager();

	public:
		/**
		 * Gets a list of visible terrain patches.
		 *
		 * @return a list of visible terrain patches
		 */
		TerrainPatchVector getVisibleTerrainPatches() final;

	public:
		/**
		 * @return the terrain currently being managed by
		 * this manager.
		 */
		Terrain& getTerrain() final;

		/**
		 * Changes the terrain the TerrainManager is managing.
		 *
		 * @param terrain the new terrain to be handled by the manager
		 */
		void setTerrain(Terrain& terrain) final;


	};

}


//
// Created by Rogiel Sulzbach on 8/12/17.
//

#pragma once

#include "XYZ/Terrain/Manager/Octree/OctreeTerrainPatch.hpp"

#include <memory>
#include <vector>

namespace XYZ::Terrain::Manager::Octree {

	/**
	 * The TerrainOctree is a structure that structures the terrain patches
	 * in a way that can be easily traversed for a list of visible patches.
	 *
	 * The Octree is also responsible for "packing" far patches into a single
	 * larger patch.
	 */
	class TerrainOctree {
	private:
		Utility::Octree<OctreeTerrainPatch*, 4> Octree;

	public:
		TerrainOctree();
		~TerrainOctree();

	};

}


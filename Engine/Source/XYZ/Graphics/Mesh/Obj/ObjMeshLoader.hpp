//
// Created by Rogiel Sulzbach on 7/2/17.
//

#pragma once

#include "XYZ/Graphics/Mesh/MeshLoader.hpp"

namespace XYZ::Graphics::Mesh::Obj {

	class ObjMeshLoader : public MeshLoader {
	public:
		/**
		 * Checks if the given resource input is supported by the loader
		 *
		 * @param resourceStream the mesh resource stream
		 *
		 * @return true if the resource is supported and can be loaded
		 * with this loader
		 */
		bool supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) override;

		/**
		 * Loads a mesh resource.
		 *
		 * @param resourceStream the mesh resource stream
		 *
		 * @return the loaded mesh
		 */
		Mesh::Ptr load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) override;

	};

}



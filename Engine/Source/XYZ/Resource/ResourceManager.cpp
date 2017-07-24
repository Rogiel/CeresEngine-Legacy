////
//// Created by Rogiel Sulzbach on 6/28/17.
////
//
//#include "ResourceManager.hpp"
//
//#include "Locator/ResourceLocator.hpp"
//
//namespace XYZ::Resource {
//
//	ResourceManager::ResourceManager(std::unique_ptr<Locator::ResourceLocator> resourceLocator)
//			: resourceLocator(std::move(resourceLocator)) {}
//
//	// -----------------------------------------------------------------------------------------------------------------
//
//	std::shared_ptr<Mesh::Mesh> ResourceManager::getMesh(const std::string& resourceName) {
//		auto found = loadedMeshes.find(resourceName);
//		if(found != loadedMeshes.end()) {
//			if(auto mesh = found->second.lock()) {
//				return mesh;
//			}
//		}
//
//		auto resourceStream = resourceLocator->locate("Meshes/" + resourceName);
//		if(resourceStream == nullptr) {
//			return nullptr;
//		}
//
//		auto mesh = meshLoader->load(std::move(resourceStream));
//		if(mesh == nullptr) {
//			return nullptr;
//		}
//
//		loadedMeshes.emplace(std::make_pair(
//				resourceName, mesh
//		));
//		return mesh;
//	}
//
//	const std::unique_ptr<Mesh::MeshLoader>& ResourceManager::getMeshLoader() const {
//		return meshLoader;
//	}
//
//	void ResourceManager::setMeshLoader(std::unique_ptr<Mesh::MeshLoader> meshLoader) {
//		ResourceManager::meshLoader = std::move(meshLoader);
//	}
//
//}
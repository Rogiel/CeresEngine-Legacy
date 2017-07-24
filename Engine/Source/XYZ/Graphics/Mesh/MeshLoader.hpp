//
// Created by Rogiel Sulzbach on 7/14/17.
//

#pragma once

#include "XYZ/Resource/ResourceLoader.hpp"
#include "Mesh.hpp"
#include "XYZ/Resource/Locator/ResourceStream.hpp"

#include <memory>

namespace XYZ::Graphics::Mesh {

	using MeshLoader = Resource::ResourceLoader<Mesh, std::unique_ptr<Resource::Locator::ResourceStream>>;

}



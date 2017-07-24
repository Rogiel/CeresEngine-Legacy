//
// Created by Rogiel Sulzbach on 7/2/17.
//

#pragma once

#include "VertexBuffer.hpp"
#include "XYZ/Graphics/Mesh/Mesh.hpp"

#include <memory>

namespace XYZ::Graphics::Renderer {

	class MeshCompiler {
	public:
		/**
		 * Compiles a mesh
		 *
		 * @return the compiled vertex buffer
		 */
		virtual std::shared_ptr<VertexBuffer> compileMesh(const Mesh::Mesh& mesh) = 0;
	};

}


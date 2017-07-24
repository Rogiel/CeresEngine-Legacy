//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

namespace XYZ::Graphics::Renderer {

	class VertexBuffer : public Resource::Resource<VertexBuffer> {
	public:
		/**
		 * Draws a mesh
		 */
		virtual void draw() = 0;

	};

}



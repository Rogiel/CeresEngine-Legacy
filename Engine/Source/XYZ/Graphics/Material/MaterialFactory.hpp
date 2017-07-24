//
// Created by Rogiel Sulzbach on 7/20/17.
//

#pragma once

#include "XYZ/Graphics/Material/PhongMaterial.hpp"
#include "XYZ/Graphics/Material/PBRMaterial.hpp"

namespace XYZ::Graphics::Material {

	class MaterialFactory {
	public:
		/**
		 * Create a new Phong shading material
		 *
		 * @return the newly created Phong material
		 */
		virtual PhongMaterial::Ptr createPhongMaterial() = 0;

//		virtual PBRMaterial::Ptr createPBRMaterial() = 0;

	};

}



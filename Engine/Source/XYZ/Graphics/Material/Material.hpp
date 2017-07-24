//
// Created by Rogiel Sulzbach on 7/20/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Shader/ShaderProgram.hpp"

namespace XYZ::Graphics::Material {

	template<typename P>
	class Material : public Resource::Resource<P> {
	public:
		/**
		 * @return the shader program implementing the material
		 */
		virtual const Shader::ShaderProgram::Ptr& getShader() = 0;

		/**
		 * Binds the shader program to the material
		 *
		 * @param shaderProgram the shader program to be bound
		 */
		virtual void bind(Shader::ShaderProgram& shaderProgram) = 0;

	};

}


//
// Created by Rogiel Sulzbach on 7/23/17.
//

#include "Shader.hpp"

#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "GeometryShader.hpp"

namespace XYZ::Graphics::Shader {

	bool AbstractShader::isVertexShader() const {
		return getShaderType() == ShaderType::VERTEX;
	}

	ShaderType VertexShader::getShaderType() const {
		return ShaderType::VERTEX;
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractShader::isFragmentShader() const {
		return getShaderType() == ShaderType::FRAGMENT;
	}

	ShaderType FragmentShader::getShaderType() const {
		return ShaderType::FRAGMENT;
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool AbstractShader::isGeometryShader() const {
		return getShaderType() == ShaderType::GEOMETRY;
	}

	ShaderType GeometryShader::getShaderType() const {
		return ShaderType::GEOMETRY;
	}

}
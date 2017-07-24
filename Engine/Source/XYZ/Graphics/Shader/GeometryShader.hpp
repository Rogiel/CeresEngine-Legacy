//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "Shader.hpp"

namespace XYZ::Graphics::Shader {

    /**
	 * A Geometry Shader is a piece of software that gets executed
     * on the GPU for every vertex of every object being rendered.
     *
     * The geometry shader is able to create new vertices by using
     * one or more vertices as a reference.
	 */
    class GeometryShader : public Shader<GeometryShader> {
	public:
		/**
		 * @return always return <tt>ShaderType::GEOMETRY</tt>
		 */
		ShaderType getShaderType() const final;
    };

}


//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "Shader.hpp"

namespace XYZ::Graphics::Shader {

    /**
	 * A Vertex Shader is a piece of software that gets executed on
     * the GPU for every vertex of every object being rendered.
	 */
    class VertexShader : public Shader<VertexShader> {
	public:
		/**
		 * @return always return <tt>ShaderType::VERTEX</tt>
		 */
		ShaderType getShaderType() const final;
    };

}


//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "Shader.hpp"

namespace XYZ::Graphics::Shader {

    /**
     * A Fragment Shader (or a Pixel Shader) is a piece of software
     * that gets executed on the GPU for every pixel displayed on the
     * screen.
     */
    class FragmentShader : public Shader<FragmentShader> {
	public:
		/**
		 * @return always return <tt>ShaderType::FRAGMENT</tt>
		 */
		ShaderType getShaderType() const final;
    };

}



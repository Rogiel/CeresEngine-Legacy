//
// Created by Rogiel Sulzbach on 7/2/17.
//

#pragma once

#include <memory>

#include "XYZ/Graphics/Shader/ShaderProgram.hpp"
#include "XYZ/Graphics/Shader/ShaderSource.hpp"

namespace XYZ::Graphics::Renderer {

    /**
     * A shader compiler is responsible for taking a ShaderSource object
     * and transform it into a renderer specific shader.
     *
     * A user can compile independently a vertex, fragment or geometry
     * shader and later combine them into a ShaderProgram that can be
     * attached into a object.
     */
    class ShaderCompiler {
    public:
        /**
         * Compiles a vertex shader
         *
         * @return the compiled vertex shader
         */
        virtual Shader::VertexShader::Ptr compileVertexShader(
                const Shader::ShaderSource& shaderSource) = 0;

        /**
         * Compiles a fragment shader
         *
         * @return the compiled fragment shader
         */
        virtual Shader::FragmentShader::Ptr compileFragmentShader(
                const Shader::ShaderSource& shaderSource) = 0;

        /**
         * Compiles a geometry shader
         *
         * @return the compiled geometry shader
         */
        virtual Shader::GeometryShader::Ptr compileGeometryShader(
                const Shader::ShaderSource& shaderSource) = 0;

    public:
        /**
         * Compiles a vertex, fragment and geometry shaders into a shader program
         *
         * @param vertexShader the vertex shader to be compiled
         * @param fragmentShader the fragment shader to be compiled
         * @param geometryShader the geometry shader to be compiled
         *
         * @return a compiled shader program
         */
        virtual std::unique_ptr<Shader::ShaderProgram> compileShaderProgram(
                const Shader::VertexShader::Ptr& vertexShader,
                const Shader::FragmentShader::Ptr& fragmentShader = nullptr,
                const Shader::GeometryShader::Ptr& geometryShader = nullptr) = 0;

    };

}


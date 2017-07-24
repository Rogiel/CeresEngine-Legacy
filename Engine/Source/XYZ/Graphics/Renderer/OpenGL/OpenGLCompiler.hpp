//
// Created by Rogiel Sulzbach on 7/8/17.
//

#pragma once

#include "XYZ/Graphics/Renderer/ShaderCompiler.hpp"
#include "XYZ/Graphics/Renderer/TextureCompiler.hpp"
#include "XYZ/Graphics/Renderer/MeshCompiler.hpp"
#include "XYZ/Graphics/Material/MaterialFactory.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLCompiler : public ShaderCompiler,
						   public TextureCompiler,
						   public MeshCompiler,
						   public Material::MaterialFactory {
	public:
		/**
         * Compiles a vertex shader
         *
         * @return the compiled vertex shader
         */
		Shader::VertexShader::Ptr compileVertexShader(
				const Shader::ShaderSource& shaderSource) override;

		/**
		 * Compiles a fragment shader
		 *
		 * @return the compiled fragment shader
		 */
		Shader::FragmentShader::Ptr compileFragmentShader(
				const Shader::ShaderSource& shaderSource) override;

		/**
		 * Compiles a geometry shader
		 *
		 * @return the compiled geometry shader
		 */
		Shader::GeometryShader::Ptr compileGeometryShader(
				const Shader::ShaderSource& shaderSource) override;

		/**
		 * Compiles a vertex, fragment and geometry shaders into a shader program
		 *
		 * @param vertexShader the vertex shader to be compiled
		 * @param fragmentShader the fragment shader to be compiled
		 * @param geometryShader the geometry shader to be compiled
		 *
		 * @return a compiled shader program
		 */
		std::unique_ptr<Shader::ShaderProgram> compileShaderProgram(
				const Shader::VertexShader::Ptr& vertexShader,
				const Shader::FragmentShader::Ptr& fragmentShader = nullptr,
				const Shader::GeometryShader::Ptr& geometryShader = nullptr) override;

	public:
		/**
         * Compiles a vertex shader
         *
         * @return the compiled vertex shader
         */
		Texture::Texture::Ptr compileTexture(
				const Texture::TextureImage& textureImage) override;

	public:
		/**
		 * Compiles a mesh
		 *
		 * @return the compiled vertex buffer
		 */
		std::shared_ptr<VertexBuffer> compileMesh(
				const Mesh::Mesh& mesh) override;

	public:
		/**
		 * Create a new Phong shading material
		 *
		 * @return the newly created Phong material
		 */
		Material::PhongMaterial::Ptr createPhongMaterial() override;

	};

}



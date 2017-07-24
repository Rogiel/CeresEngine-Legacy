//
// Created by Rogiel Sulzbach on 7/21/17.
//

#pragma once

#include <XYZ/Graphics/Renderer/Renderer.hpp>
#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Mesh/Mesh.hpp"
#include "XYZ/Graphics/Material/Material.hpp"

namespace XYZ::Graphics::Model {

	class Model : public Resource::Resource<Model> {
	private:
		/**
		 * The model mesh object
		 */
		Mesh::Mesh::Ptr mesh;

		/**
		 * The model compiled vertex buffer
		 */
		Renderer::VertexBuffer::Ptr vertexBuffer;

	private:
//		/**
//		 * The model material
//		 */
//		Material::Material::Ptr material;

	public:
		/**
		 * Creates a new empty model
		 */
		Model();

		/**
		 * Destroys the model and release all its resources
		 */
		~Model();

	public:
		/**
		 * Compiles the model shader, mesh and other resources.
		 */
		void compile();

		/**
		 * Renders the model.
		 *
		 * This method can only be called from a renderer context.
		 *
		 * @param renderer the renderer context
		 */
		void render(Renderer::Renderer& renderer);

	public:
		/**
		 * @return the model mesh object
		 */
		const Mesh::Mesh::Ptr& getMesh() const;

		/**
		 * @param mesh the model mesh object
		 */
		void setMesh(const Mesh::Mesh::Ptr& mesh);

//		/**
//		 * @return the model material
//		 */
//		const Material::Material::Ptr& getMaterial() const;
//
//		/**
//		 * @param material the model material
//		 */
//		void setMaterial(const Material::Material::Ptr& material);

		/**
		 * @return the model compiled vertex buffer
		 */
		const Renderer::VertexBuffer::Ptr& getVertexBuffer() const;

		/**
		 * @param vertexBuffer the model compiled vertex buffer
		 */
		void setVertexBuffer(const Renderer::VertexBuffer::Ptr& vertexBuffer);

		/**
		 * @return the model compiled shader program
		 */
		const Shader::ShaderProgram::Ptr& getShaderProgram() const;

		/**
		 * @param shaderProgram the model compiled shader program
		 */
		void setShaderProgram(const Shader::ShaderProgram::Ptr& shaderProgram);


	};

}


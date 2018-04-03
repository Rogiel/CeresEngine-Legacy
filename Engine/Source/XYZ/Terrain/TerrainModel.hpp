//
// Created by Rogiel Sulzbach on 8/6/17.
//

#pragma once

#include "XYZ/Graphics/Model/Model.hpp"
#include "XYZ/Terrain/Terrain.hpp"

#include "XYZ/Graphics/Mesh/Mesh.hpp"
#include "XYZ/Graphics/Texture/Texture.hpp"

namespace XYZ::Terrain {

	class TerrainModel : public Graphics::Model::Model {
	private:
		/**
		 * The owning terrain
		 */
		Terrain::Ptr terrain;

		/**
		 * The chunk coordinate
		 */
		Terrain::ChunkCoordinate chunkCoordinate;

	private:
		std::vector<Graphics::Mesh::Mesh::Ptr> meshes;

		/**
		 * The terrain mesh
		 */
		std::vector<Graphics::Renderer::VertexBuffer::Ptr> surfaceMesh;

		std::vector<Graphics::Texture::TextureImage::Ptr> images;

		/**
		 * The terrain texture
		 */
		std::vector<Graphics::Texture::Texture::Ptr> texture;

	public:
		/**
		 * Creates a new terrain model
		 *
		 * @param terrain the terrain to generate textures and meshes from
		 * @param chunkCoordinate the chunk coordinate
		 */
		TerrainModel(Terrain::Ptr terrain, Terrain::ChunkCoordinate chunkCoordinate);

	public:
		/**
		 * Renders the model.
		 *
		 * This method can only be called from a renderer context.
		 *
		 * @param renderer the renderer context
		 */
		void render(Graphics::Renderer::Renderer& renderer, const Graphics::Model::LevelOfDetail& levelOfDetail) final override;

		/**
		 * Sets the shader uniform variables for the model material
		 *
		 * @param shader the shader to set uniforms to
		 */
		void setMaterialShaderUniforms(Graphics::Renderer::Renderer& renderer, Graphics::Shader::ShaderProgram& shader,
									   const Graphics::Model::LevelOfDetail& levelOfDetail) final override;

		virtual glm::vec3 getSize() final override;

	private:
		int computeLOD(const Graphics::Model::LevelOfDetail& levelOfDetail);
		void loadIfNeeded(Graphics::Renderer::Renderer& renderer, int lod);


	};

}



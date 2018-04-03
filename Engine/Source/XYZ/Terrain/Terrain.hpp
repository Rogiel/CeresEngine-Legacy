//
// Created by Rogiel Sulzbach on 7/31/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Model/Model.hpp"
#include "XYZ/Graphics/Texture/TextureImage.hpp"
#include "XYZ/Graphics/Mesh/Mesh.hpp"

namespace XYZ::Terrain {

	class Terrain : public Resource::Resource<Terrain> {
	public:
		using Height = double;
		using Coordinate = double;

		using ChunkCoordinate = std::tuple<Coordinate, Coordinate, Coordinate, Coordinate>;
		using LevelOfDetail = unsigned int;

		using Color = glm::vec4;
		using Normal = glm::vec3;

	public:
		/**
		 * Gets the terrain height at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain height at the given position
		 */
		virtual Height getHeight(Coordinate x, Coordinate y) = 0;

		/**
		 * Gets the terrain diffuse color at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain diffuse color at the given position
		 */
		virtual Color getDiffuseColor(Coordinate x, Coordinate y) = 0;

		/**
		 * Gets the terrain specular color at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain specular color at the given position
		 */
		virtual Color getSpecularColor(Coordinate x, Coordinate y) = 0;

		/**
		 * Gets the terrain normal vector at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain normal vector at the given position
		 */
		virtual Normal getNormal(Coordinate x, Coordinate y) = 0;

	public:
		/**
		 * Creates a new terrain model for the chunk given by the box at
		 * [x0, x1] and [y0, y1].
		 *
		 * This model will automatically adapt to LevelOfDetail changes.
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 *
		 * @return a newly created model containing the terrain
		 */
		virtual Graphics::Model::Model::Ptr createModel(ChunkCoordinate chunkCoordinate) = 0;

		/**
		 * Creates a new terrain surface mesh for the given terrain chunk.
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created terrain surface mesh for the given chunk coordinate
		 */
		virtual Graphics::Mesh::Mesh::Ptr createSurfaceMesh(ChunkCoordinate chunkCoordinate,
															LevelOfDetail levelOfDetail) = 0;

		/**
		 * Creates a new diffuse terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		virtual Graphics::Texture::TextureImage::Ptr createDiffuseTexture(ChunkCoordinate chunkCoordinate,
																		  LevelOfDetail levelOfDetail) = 0;

		/**
		 * Creates a new specular terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		virtual Graphics::Texture::TextureImage::Ptr createSpecularTexture(ChunkCoordinate chunkCoordinate,
																		   LevelOfDetail levelOfDetail) = 0;

		/**
		 * Creates a new normal terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		virtual Graphics::Texture::TextureImage::Ptr createNormalTexture(ChunkCoordinate chunkCoordinate,
																		 LevelOfDetail levelOfDetail) = 0;


	};

}



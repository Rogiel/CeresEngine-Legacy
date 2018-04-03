//
// Created by Rogiel Sulzbach on 7/17/17.
//

#pragma once

#include "XYZ/Terrain/Terrain.hpp"

#include <vector>

#include "XYZ/Graphics/Mesh/Mesh.hpp"
#include "XYZ/Graphics/Model/Model.hpp"

namespace XYZ::Terrain::Plain {

	class PlainTerrain : public Terrain {
	public:
		PlainTerrain();

	public:
		/**
		 * Gets the terrain height at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain height at the given position
		 */
		Height getHeight(double x, double y) final override;

		/**
		 * Gets the terrain diffuse color at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain diffuse color at the given position
		 */
		Color getDiffuseColor(Coordinate x, Coordinate y) final override;

		/**
		 * Gets the terrain specular color at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain specular color at the given position
		 */
		Color getSpecularColor(Coordinate x, Coordinate y) final override;

		/**
		 * Gets the terrain normal vector at the given position
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 *
		 * @return the terrain normal vector at the given position
		 */
		Normal getNormal(Coordinate x, Coordinate y) final override;

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
		Graphics::Model::Model::Ptr createModel(ChunkCoordinate chunkCoordinate) final override;

		/**
		 * Creates a new terrain surface mesh for the given terrain chunk.
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created terrain surface mesh for the given chunk coordinate
		 */
		Graphics::Mesh::Mesh::Ptr createSurfaceMesh(ChunkCoordinate chunkCoordinate,
													LevelOfDetail levelOfDetail) final override;

		/**
		 * Creates a new diffuse terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		Graphics::Texture::TextureImage::Ptr createDiffuseTexture(ChunkCoordinate chunkCoordinate,
																  LevelOfDetail levelOfDetail) final override;

		/**
		 * Creates a new specular terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		Graphics::Texture::TextureImage::Ptr createSpecularTexture(ChunkCoordinate chunkCoordinate,
																   LevelOfDetail levelOfDetail) final override;

		/**
		 * Creates a new normal terrain texture image for the given terrain chunk
		 *
		 * @param chunkCoordinate the terrain chunk coordinate
		 * @param levelOfDetail the terrain level of detail
		 *
		 * @return a newly created texture for the given chunk coordinate
		 */
		Graphics::Texture::TextureImage::Ptr createNormalTexture(ChunkCoordinate chunkCoordinate,
																 LevelOfDetail levelOfDetail) final override;


	public:
		Graphics::Model::Model::Ptr getChunk(double x, double y, unsigned int resolution);

		std::vector<char>
		generateTexture(int w, int h, double south = -90, double north = 90, double west = -180, double east = 180);

		std::vector<char>
		generateNormal(int w, int h, double south = -90, double north = 90, double west = -180, double east = 180);

		Graphics::Mesh::Mesh::Ptr
		generateMesh(unsigned int resolution, double south = -90, double north = 90, double west = -180,
					 double east = 180);

	};

}




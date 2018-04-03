//
// Created by Rogiel Sulzbach on 7/17/17.
//

#include "NoiseTerrain.hpp"

#include <iostream>
#include <vector>
#include <XYZ/Graphics/Mesh/Mesh.hpp>
#include "XYZ/Terrain/TerrainModel.hpp"

#include "SimplexNoise.hpp"

#include <glm/ext.hpp>

namespace XYZ::Terrain::Noise {

	NoiseTerrain::NoiseTerrain() {

	}

	SimplexNoise simplexNoise(0.1f, 1.0f);

	// -----------------------------------------------------------------------------------------------------------------

	Terrain::Height NoiseTerrain::getHeight(double x, double y) {
		return simplexNoise.fractal(8, float(x / 1000.0f), float(y / 1000.0f)) * 20.0f;
	}

	Terrain::Color NoiseTerrain::getDiffuseColor(Coordinate x, Coordinate y) {
		float v = simplexNoise.fractal(8, float(x / 1000.0f), float(y / 1000.0f));

		if(v >= 0.8) {
			return glm::vec4(v, 1.0, v, 1.0);
		} else if(v > 0.0) {
			return glm::vec4(0.0, 1.0 - v, 0.0, 1.0);
		} else {
			return glm::vec4(0.0, 0.0, 1.0 + v, 1.0);
		}

//		return glm::normalize(glm::vec4(v, v, v, 1.0));
	}

	Terrain::Color NoiseTerrain::getSpecularColor(Coordinate x, Coordinate y) {
		return Color(0.0);
	}

	Terrain::Normal NoiseTerrain::getNormal(Coordinate x, Coordinate y) {
		return Normal(0.0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	Graphics::Model::Model::Ptr NoiseTerrain::createModel(ChunkCoordinate chunkCoordinate) {
		return std::make_shared<TerrainModel>(shared_from_this(), chunkCoordinate);
	}

	static unsigned int MeshDetailLevels[] = {
			128, // 0
			128, // 1
			128, // 2
			64,  // 3
			32,  // 4
			16,  // 5
			4,  // 6
			4,  // 7
	};

	Graphics::Mesh::Mesh::Ptr NoiseTerrain::createSurfaceMesh(ChunkCoordinate chunkCoordinate,
															  LevelOfDetail levelOfDetail) {
		double x0 = std::get<0>(chunkCoordinate);
		double x1 = std::get<1>(chunkCoordinate);
		double y0 = std::get<2>(chunkCoordinate);
		double y1 = std::get<3>(chunkCoordinate);

		return generateMesh(MeshDetailLevels[levelOfDetail], x0, x1, y0, y1);
	}

	static unsigned int DetailLevels[] = {
			512, // 0
			256, // 1
			128, // 2
			64,  // 3
			32,  // 4
			16,  // 5
			4,  // 6
			1,  // 7
	};

	Graphics::Texture::TextureImage::Ptr NoiseTerrain::createDiffuseTexture(ChunkCoordinate chunkCoordinate,
																			LevelOfDetail levelOfDetail) {
		double x0 = std::get<2>(chunkCoordinate);
		double x1 = std::get<3>(chunkCoordinate);
		double y0 = std::get<0>(chunkCoordinate);
		double y1 = std::get<1>(chunkCoordinate);

		float dx = float(x1 - x0);
		float dy = float(y1 - y0);

		unsigned int resolution = DetailLevels[levelOfDetail];
		float resolutionf = float(resolution);

		std::vector<char> texture(resolution * resolution * 3);
		for(int y = 0; y < resolution; y++) {
			for(int x = 0; x < resolution; x++) {
				auto color = getDiffuseColor(x0 + float(x) * dx / (resolutionf - 1), y0 + float(y) * dy / (resolutionf - 1));
//				auto color = glm::vec3(0.5);

				texture[(y * resolution + x) * 3 + 0] = char(color.r * 255.0);
				texture[(y * resolution + x) * 3 + 1] = char(color.g * 255.0);
				texture[(y * resolution + x) * 3 + 2] = char(color.b * 255.0);
			}
		}

		return std::make_shared<Graphics::Texture::TextureImage>(resolution, resolution, std::move(texture), false);
	}

	Graphics::Texture::TextureImage::Ptr NoiseTerrain::createSpecularTexture(ChunkCoordinate chunkCoordinate,
																			 LevelOfDetail levelOfDetail) {
		return nullptr;
	}

	Graphics::Texture::TextureImage::Ptr NoiseTerrain::createNormalTexture(ChunkCoordinate chunkCoordinate,
																		   LevelOfDetail levelOfDetail) {
		return nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------

	XYZ::Graphics::Mesh::Mesh::Ptr NoiseTerrain::generateMesh(unsigned int resolution, double south, double north,
															  double west, double east) {
		using Graphics::Mesh::Vertex;
		std::vector<Vertex> vertices((resolution + 1) * (resolution + 1));
		std::vector<unsigned int> indices;

		float dx = float(east - west);
		float dy = float(north - south);

		float resolutionf = float(resolution);

		// Write the elevation mesh
		for(int x = 0; x < resolution; x++) {
			for(int y = 0; y < resolution; y++) {
				float vx = float(x) / (resolutionf - 1) * dx;
				float vz = float(y) / (resolutionf - 1) * dy;

				float height = (float) getHeight(west + vx, south + vz);
				float vy = height;
				if(vy < 0.0) {
					vy = 0.0;
				}
//				float height = 0.0;

				float heightL = (float) getHeight(float(x-1) / (resolutionf - 1) * dx, vz);
				float heightR = (float) getHeight(float(x+1) / (resolutionf - 1) * dx, vz);
				float heightD = (float) getHeight(vx, float(y - 1) / (resolutionf - 1) * dy);
				float heightU = (float) getHeight(vx, float(y + 1) / (resolutionf - 1) * dy);

				glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0, heightD - heightU));

				vertices[y * resolution + x] = Vertex {
						glm::vec3(vx, vy, vz), /* position */
						normal, /* normal */
						glm::vec2(float(x) / (resolutionf - 1), float(y) / (resolutionf - 1)), /* texCoords */
						glm::vec3(1.0, 0.0, 0.0) /* tangent */
				};
			}
		}

		for(int y = 0; y < resolution - 1; y++) {
			for(int x = 0; x < resolution - 1; x++) {
				const unsigned int topLeft = (y * resolution) + x;
				const unsigned int topRight = topLeft + 1;
				const unsigned int bottomLeft = ((y + 1) * resolution) + x;
				const unsigned int bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}
		return std::make_shared<Graphics::Mesh::Mesh>(indices, vertices);
	}

}
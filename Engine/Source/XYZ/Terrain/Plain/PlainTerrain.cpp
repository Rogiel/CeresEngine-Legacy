//
// Created by Rogiel Sulzbach on 7/17/17.
//

#include "PlainTerrain.hpp"

#include <iostream>
#include <vector>
#include <XYZ/Graphics/Mesh/Mesh.hpp>
#include "XYZ/Terrain/TerrainModel.hpp"

#include <glm/ext.hpp>

namespace XYZ::Terrain::Plain {

	PlainTerrain::PlainTerrain() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	Terrain::Height PlainTerrain::getHeight(double x, double y) {
		return 0;
	}

	Terrain::Color PlainTerrain::getDiffuseColor(Coordinate x, Coordinate y) {
		return glm::vec4(0.0, 1.0, 0.0, 1.0);
	}

	Terrain::Color PlainTerrain::getSpecularColor(Coordinate x, Coordinate y) {
		return Color(0.0);
	}

	Terrain::Normal PlainTerrain::getNormal(Coordinate x, Coordinate y) {
		return Normal(0.0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	Graphics::Model::Model::Ptr PlainTerrain::createModel(ChunkCoordinate chunkCoordinate) {
		return std::make_shared<TerrainModel>(shared_from_this(), chunkCoordinate);
	}

	Graphics::Mesh::Mesh::Ptr PlainTerrain::createSurfaceMesh(ChunkCoordinate chunkCoordinate,
															  LevelOfDetail levelOfDetail) {
		double x0 = std::get<0>(chunkCoordinate);
		double x1 = std::get<1>(chunkCoordinate);
		double y0 = std::get<2>(chunkCoordinate);
		double y1 = std::get<3>(chunkCoordinate);

		return generateMesh(4, x0, x1, y0, y1);
	}

	static unsigned int DetailLevels[] = {
			1,  // 7
	};

	Graphics::Texture::TextureImage::Ptr PlainTerrain::createDiffuseTexture(ChunkCoordinate chunkCoordinate,
																			LevelOfDetail levelOfDetail) {
		double x0 = std::get<0>(chunkCoordinate);
		double x1 = std::get<1>(chunkCoordinate);
		double y0 = std::get<2>(chunkCoordinate);
		double y1 = std::get<3>(chunkCoordinate);

		float dx = float(x1 - x0);
		float dy = float(y1 - y0);

		unsigned int resolution = 1;
		float resolutionf = float(resolution);

		std::vector<char> texture(resolution * resolution * 3);
		for(int y = 0; y < resolution; y++) {
			for(int x = 0; x < resolution; x++) {
				auto color = getDiffuseColor(x0 + float(x) * dx / resolutionf, y0 + float(y) * dy / resolutionf);

				texture[(y * resolution + x) * 3 + 0] = char(color.r * 255.0);
				texture[(y * resolution + x) * 3 + 1] = char(color.g * 255.0);
				texture[(y * resolution + x) * 3 + 2] = char(color.b * 255.0);
			}
		}

		return std::make_shared<Graphics::Texture::TextureImage>(resolution, resolution, std::move(texture), false);
	}

	Graphics::Texture::TextureImage::Ptr PlainTerrain::createSpecularTexture(ChunkCoordinate chunkCoordinate,
																			 LevelOfDetail levelOfDetail) {
		return nullptr;
	}

	Graphics::Texture::TextureImage::Ptr PlainTerrain::createNormalTexture(ChunkCoordinate chunkCoordinate,
																		   LevelOfDetail levelOfDetail) {
		return nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::vector<char> PlainTerrain::generateTexture(int w, int h, double south, double north, double west,
													double east) {
		float dx = float(east - west);
		float dy = float(north - south);

		float resolutionf = float(w);

		std::vector<char> texture(w * h * 3);
		for(int y = 0; y < w; y++) {
			for(int x = 0; x < h; x++) {
				float v = 0;

				texture[(y * w + x) * 3 + 0] = char(v * 255);
				texture[(y * w + x) * 3 + 1] = char(255);
				texture[(y * w + x) * 3 + 2] = char(v * 255);
			}
		}

		return texture;
	}

	XYZ::Graphics::Mesh::Mesh::Ptr PlainTerrain::generateMesh(unsigned int resolution, double south, double north,
															  double west, double east) {
		using Graphics::Mesh::Vertex;
		std::vector<Vertex> vertices(resolution * resolution);
		std::vector<unsigned int> indices;
		indices.reserve(6 * (resolution - 1) * (resolution - 1));

		float dx = float(east - west);
		float dy = float(north - south);

		float resolutionf = float(resolution);

		// Write the elevation mesh
		unsigned int vertexIndex = 0;

		for(int x = 0; x < resolution; x++) {
			for(int y = 0; y < resolution; y++) {
				float height = 0;

				vertices[y * resolution + x] = Vertex {
						glm::vec3(float(x) / (resolutionf - 1) * dx, height,
								  float(y) / (resolutionf - 1) * dy), /* position */
						glm::vec3(0, 1.0, 0.0), /* normal */
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


//		for(int y = 0; y < resolution + 1; y++) {
//			for(int x = 0; x < resolution + 1; x++) {
//				float height = 0;
//
//				vertices[y * resolution + x] = Vertex {
//						glm::vec3(float(x) * dx / resolutionf, height,
//								  float(y) * dy / resolutionf), /* position */
//						glm::vec3(0, 1.0, 0.0), /* normal */
//						glm::vec2(float(x) / resolutionf, float(y) / resolutionf), /* texCoords */
//						glm::vec3(1.0, 0.0, 0.0)  /* tangent */
//				};
//
//				if((x >= 0 && y >= 0) && (x <= resolution && y <= resolution)) {
//					unsigned int quad[4] = {
//							(unsigned int) (y + 0) * resolution + (x + 0),
//							(unsigned int) (y + 0) * resolution + (x + 1),
//							(unsigned int) (y + 1) * resolution + (x + 0),
//							(unsigned int) (y + 1) * resolution + (x + 1)
//					};
//
//					indices.push_back(quad[0]);
//					indices.push_back(quad[1]);
//					indices.push_back(quad[2]);
//
//					indices.push_back(quad[2]);
//					indices.push_back(quad[1]);
//					indices.push_back(quad[3]);
//				}
//
//				vertexIndex++;
//			}
//		}

		return std::make_shared<Graphics::Mesh::Mesh>(indices, vertices);
	}

}
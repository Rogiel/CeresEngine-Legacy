//
// Created by Rogiel Sulzbach on 8/6/17.
//

#include "TerrainModel.hpp"

#include "XYZ/Graphics/Renderer/Renderer.hpp"
#include "XYZ/Graphics/Renderer/MeshCompiler.hpp"
#include "XYZ/Graphics/Renderer/TextureCompiler.hpp"

#include <iostream>

#include <glm/ext.hpp>

namespace XYZ::Terrain {

	TerrainModel::TerrainModel(Terrain::Ptr terrain, Terrain::ChunkCoordinate chunkCoordinate) :
			terrain(terrain), chunkCoordinate(chunkCoordinate), meshes(8), surfaceMesh(8), images(8), texture(8) {
	}

	// -----------------------------------------------------------------------------------------------------------------

	void TerrainModel::render(Graphics::Renderer::Renderer& renderer, const Graphics::Model::LevelOfDetail& levelOfDetail) {
		auto lod = computeLOD(levelOfDetail);
		if(lod == 8) {
			return;
		}
		loadIfNeeded(renderer, lod);
		surfaceMesh[lod]->draw();
	}

	void TerrainModel::setMaterialShaderUniforms(Graphics::Renderer::Renderer& renderer,
												 Graphics::Shader::ShaderProgram& shader,
												 const Graphics::Model::LevelOfDetail& levelOfDetail) {
		auto lod = computeLOD(levelOfDetail);
		if(lod == 8) {
			return;
		}

		loadIfNeeded(renderer, lod);

		shader.set("material.shininess", 32.0f);
//		shader.set("material.diffuseColor", glm::vec3(float(lod) / 8.0, 0.0, 0.0));
//		shader.set("material.specularColor", specularColor);

		// If there is a texture, activate it.
		if(texture[lod] != nullptr) {
			texture[lod]->activate(0);
			shader.set("material.diffuse", 0);
			shader.set("material.specular", 0);
		}

		shader.set("material.hasNormalMap", false);
	}

	glm::vec3 TerrainModel::getSize() {
		return glm::vec3(std::get<2>(chunkCoordinate), 0.0, std::get<0>(chunkCoordinate));
	}

	// -----------------------------------------------------------------------------------------------------------------

	int TerrainModel::computeLOD(const Graphics::Model::LevelOfDetail& levelOfDetail) {
//		auto distance = glm::dot(glm::abs(glm::vec3(0.0) - levelOfDetail.screenSize), glm::vec3(1.0, 1.0, 1.0)) * 0.5f / 10.0f;
//		if(distance == 0.0) {
//			return 8;
//		}
////		if(getSize() == glm::vec3(0.0, 0.0, 0.0)) {
////			std::cout << to_string(levelOfDetail.screenSize) << std::endl;
////			std::cout << std::get<2>(chunkCoordinate) << ", " << std::get<0>(chunkCoordinate) << std::endl;
////			std::cout << distance << std::endl;
////		}
//
//		if(distance >= -1 && distance < 2) {
//			return 0;
//		} else if(distance >= 2 && distance < 3) {
//			return 1;
//		} else if(distance >= 3 && distance < 4) {
//			return 2;
//		} else if(distance >= 4 && distance < 5) {
//			return 3;
//		} else if(distance >= 5 && distance < 6) {
//			return 4;
//		} else if(distance >= 6 && distance < 7) {
//			return 5;
//		} else if(distance >= 7 && distance < 8) {
//			return 6;
//		} else if(distance >= 8 && distance < 9) {
//			return 7;
//		} else {
//			return 8;
//		}
		return 3;
	}

	void TerrainModel::loadIfNeeded(Graphics::Renderer::Renderer& renderer, int lod) {
		// try to locate a LOD terrain
		if(surfaceMesh[lod] == nullptr) {
			auto mesh = terrain->createSurfaceMesh(chunkCoordinate, lod);
			surfaceMesh[lod] = renderer.getMeshCompiler().compileMesh(*mesh);
			meshes[lod] = mesh;
		}

		if(texture[lod] == nullptr) {
			auto image = terrain->createDiffuseTexture(chunkCoordinate, lod);
			texture[lod] = renderer.getTextureCompiler().compileTexture(*image);
			texture[lod]->setMagnificationMinificationFilter(Graphics::Texture::TextureMagnification::LINEAR,
															 Graphics::Texture::TextureMinification::LINEAR);
			texture[lod]->setWrapMode(Graphics::Texture::TextureWrap::CLAMP_TO_EDGE,
									  Graphics::Texture::TextureWrap::CLAMP_TO_EDGE);
			images[lod] = image;
		}
	}

}

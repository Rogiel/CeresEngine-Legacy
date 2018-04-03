//
// Created by Rogiel Sulzbach on 7/29/17.
//

#include "StaticModel.hpp"

#include "XYZ/Graphics/Renderer/Renderer.hpp"

namespace XYZ::Graphics::Model {

	StaticModel::StaticModel(Mesh::Mesh::Ptr mesh,
							 Texture::Texture::Ptr diffuseTexture,
							 Texture::Texture::Ptr specularTexture,
							 float shininess,
							 Texture::Texture::Ptr normalMap,
							 bool castShadows) :
			mesh(std::move(mesh)),
			diffuseTexture(std::move(diffuseTexture)),
			specularTexture(std::move(specularTexture)),
			shininess(shininess),
			normalMap(std::move(normalMap)),
			castShadows(castShadows) {
	}

	// -----------------------------------------------------------------------------------------------------------------

	void StaticModel::render(Renderer::Renderer& renderer, const LevelOfDetail& levelOfDetail) {
		if(vertexBuffer == nullptr) {
			vertexBuffer = renderer.getMeshCompiler().compileMesh(*mesh);
		}
		vertexBuffer->draw();
	}

	void StaticModel::setMaterialShaderUniforms(Renderer::Renderer& renderer, Shader::ShaderProgram& shader,
												const LevelOfDetail& levelOfDetail) {
		shader.set("material.shininess", shininess);
		shader.set("material.diffuseColor", diffuseColor);
		shader.set("material.specularColor", specularColor);

		// If there is a texture, activate it.
		if(diffuseTexture) {
			diffuseTexture->activate(0);
			shader.set("material.diffuse", 0);
		}

		if(specularTexture) {
			specularTexture->activate(1);
			shader.set("material.specular", 1);
		}

		if(normalMap) {
			normalMap->activate(2);
			shader.set("material.normal", 2);
			shader.set("material.hasNormalMap", true);
		} else {
			shader.set("material.hasNormalMap", false);
		}
	}

	glm::vec3 StaticModel::getSize() {
		return glm::vec3(0.0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	const Mesh::Mesh::Ptr& StaticModel::getMesh() const {
		return mesh;
	}

	void StaticModel::setMesh(const Mesh::Mesh::Ptr& mesh) {
		StaticModel::mesh = mesh;
	}

	const Renderer::VertexBuffer::Ptr& StaticModel::getVertexBuffer() const {
		return vertexBuffer;
	}

	void StaticModel::setVertexBuffer(const Renderer::VertexBuffer::Ptr& vertexBuffer) {
		StaticModel::vertexBuffer = vertexBuffer;
	}

	const glm::vec3& StaticModel::getDiffuseColor() const {
		return diffuseColor;
	}

	void StaticModel::setDiffuseColor(const glm::vec3& diffuseColor) {
		StaticModel::diffuseColor = diffuseColor;
	}

	const Texture::Texture::Ptr& StaticModel::getDiffuseTexture() const {
		return diffuseTexture;
	}

	void StaticModel::setDiffuseTexture(const Texture::Texture::Ptr& diffuseTexture) {
		StaticModel::diffuseTexture = diffuseTexture;
	}

	const glm::vec3& StaticModel::getSpecularColor() const {
		return specularColor;
	}

	void StaticModel::setSpecularColor(const glm::vec3& specularColor) {
		StaticModel::specularColor = specularColor;
	}

	const Texture::Texture::Ptr& StaticModel::getSpecularTexture() const {
		return specularTexture;
	}

	void StaticModel::setSpecularTexture(const Texture::Texture::Ptr& specularTexture) {
		StaticModel::specularTexture = specularTexture;
	}

	float StaticModel::getShininess() const {
		return shininess;
	}

	void StaticModel::setShininess(float shininess) {
		StaticModel::shininess = shininess;
	}

	const Texture::Texture::Ptr& StaticModel::getNormalMap() const {
		return normalMap;
	}

	void StaticModel::setNormalMap(const Texture::Texture::Ptr& normalMap) {
		StaticModel::normalMap = normalMap;
	}

	bool StaticModel::shouldCastShadows() const {
		return castShadows;
	}

	void StaticModel::setCastShadows(bool castShadows) {
		StaticModel::castShadows = castShadows;
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool StaticModel::didReceiveMemoryWarning() {
		// if the vertex buffer is already loaded, we can remove the mesh
		if(vertexBuffer != nullptr) {
			mesh = nullptr;
		}

		// dont ever delete the model!
		return false;
	}
}

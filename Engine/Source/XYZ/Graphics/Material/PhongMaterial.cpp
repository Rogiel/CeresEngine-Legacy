//
// Created by Rogiel Sulzbach on 7/20/17.
//

#include "PhongMaterial.hpp"

namespace XYZ::Graphics::Material {

	void PhongMaterial::bind(Shader::ShaderProgram& shaderProgram) {
		unsigned int slot = 0;

		shaderProgram.set("material.diffuseColor", diffuseColor);
		if(diffuse != nullptr) {
			diffuse->activate(slot);
			shaderProgram.set("material.diffuse", slot);
			slot++;
		}

		shaderProgram.set("material.specularColor", specularColor);
		if(specular != nullptr) {
			specular->activate(slot);
			shaderProgram.set("material.specular", slot);
			slot++;
		}

		shaderProgram.set("material.shininess", shininess);
		shaderProgram.set("material.blinn", blinn);

		if(normalMap != nullptr) {
			normalMap->activate(slot);
			shaderProgram.set("material.normal", slot);
			shaderProgram.set("material.hasNormalMap", true);
			slot++;
		} else {
			shaderProgram.set("material.hasNormalMap", false);
		}

		if(displacementMap != nullptr) {
			displacementMap->activate(slot);
			shaderProgram.set("material.displacementMap", slot);
			shaderProgram.set("material.hasDisplacementMap", true);
			slot++;
		} else {
			shaderProgram.set("material.hasDisplacementMap", false);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------
	
	const glm::vec3& PhongMaterial::getDiffuseColor() const {
		return diffuseColor;
	}

	void PhongMaterial::setDiffuseColor(const glm::vec3& diffuseColor) {
		PhongMaterial::diffuseColor = diffuseColor;
	}

	const Texture::Texture::Ptr& PhongMaterial::getDiffuse() const {
		return diffuse;
	}

	void PhongMaterial::setDiffuse(const Texture::Texture::Ptr& diffuse) {
		PhongMaterial::diffuse = diffuse;
	}

	const glm::vec3& PhongMaterial::getSpecularColor() const {
		return specularColor;
	}

	void PhongMaterial::setSpecularColor(const glm::vec3& specularColor) {
		PhongMaterial::specularColor = specularColor;
	}

	const Texture::Texture::Ptr& PhongMaterial::getSpecular() const {
		return specular;
	}

	void PhongMaterial::setSpecular(const Texture::Texture::Ptr& specular) {
		PhongMaterial::specular = specular;
	}

	float PhongMaterial::getShininess() const {
		return shininess;
	}

	void PhongMaterial::setShininess(float shininess) {
		PhongMaterial::shininess = shininess;
	}

	bool PhongMaterial::isBlinn() const {
		return blinn;
	}

	void PhongMaterial::setBlinn(bool blinn) {
		PhongMaterial::blinn = blinn;
	}

	const Texture::Texture::Ptr& PhongMaterial::getNormalMap() const {
		return normalMap;
	}

	void PhongMaterial::setNormalMap(const Texture::Texture::Ptr& normalMap) {
		PhongMaterial::normalMap = normalMap;
	}

	const Texture::Texture::Ptr& PhongMaterial::getDisplacementMap() const {
		return displacementMap;
	}

	void PhongMaterial::setDisplacementMap(const Texture::Texture::Ptr& displacementMap) {
		PhongMaterial::displacementMap = displacementMap;
	}

}
//
// Created by Rogiel Sulzbach on 7/20/17.
//

#pragma once

#include "XYZ/Graphics/Material/Material.hpp"

#include "XYZ/Graphics/Texture/Texture.hpp"
#include "XYZ/Graphics/Shader/ShaderProgram.hpp"

#include <glm/vec3.hpp>

namespace XYZ::Graphics::Material {

	class PhongMaterial : public Material<PhongMaterial> {
	private:
		/**
		 * The material base diffuse color.
		 *
		 * Will be added with the diffuse texture.
		 */
		glm::vec3 diffuseColor;

		/**
		 * The material diffuse texture
		 */
		Texture::Texture::Ptr diffuse;

		/**
		 * The material base specular color.
		 *
		 * Will be added with the specular texture.
		 */
		glm::vec3 specularColor;

		/**
		 * The material specular map texture
		 */
		Texture::Texture::Ptr specular;

		/**
		 * The material shininess factor
		 */
		float shininess;

		/**
		 * A flag indicating the material should use the "Phong-Blinn" model
		 * simplification instead of the standard Phong model.
		 */
		bool blinn;

	private:
		/**
		 * The material normal map texture in tangent space
		 */
		Texture::Texture::Ptr normalMap;

		/**
		 * The material displacement map texture in tangent space
		 */
		Texture::Texture::Ptr displacementMap;

	public:
		/**
		 * Binds the shader program to the material
		 *
		 * @param shaderProgram the shader program to be bound
		 */
		void bind(Shader::ShaderProgram& shaderProgram) override;

	public:
		/**
		 * @return the material base diffuse color
		 */
		const glm::vec3& getDiffuseColor() const;

		/**
		 * @param diffuseColor the material base diffuse color
		 */
		void setDiffuseColor(const glm::vec3& diffuseColor);

		/**
		 * @return the material diffuse texture
		 */
		const Texture::Texture::Ptr& getDiffuse() const;

		/**
		 * @param diffuse the material diffuse texture
		 */
		void setDiffuse(const Texture::Texture::Ptr& diffuse);

		/**
		 * @return the material base specular color
		 */
		const glm::vec3& getSpecularColor() const;

		/**
		 * @param specularColor the material base specular color
		 */
		void setSpecularColor(const glm::vec3& specularColor);

		/**
		 * @return the material base specular texture
		 */
		const Texture::Texture::Ptr& getSpecular() const;

		/**
		 * @param specular the material base specular texture
		 */
		void setSpecular(const Texture::Texture::Ptr& specular);

		/**
		 * @return the material shininess factor
		 */
		float getShininess() const;

		/**
		 * @param shininess the material shininess factor
		 */
		void setShininess(float shininess);

		/**
		 * @return a flag indicating the material should use the "Phong-Blinn" model
		 * simplification instead of the standard Phong model.
		 */
		bool isBlinn() const;

		/**
		 * @param blinn a flag indicating the material should use the "Phong-Blinn" model
		 * simplification instead of the standard Phong model.
		 */
		void setBlinn(bool blinn);

		/**
		 * @return the material normal map texture in tangent space
		 */
		const Texture::Texture::Ptr& getNormalMap() const;

		/**
		 * @param normalMap the material normal map texture in tangent space
		 */
		void setNormalMap(const Texture::Texture::Ptr& normalMap);

		/**
		 * @return the material displacement map texture in tangent space
		 */
		const Texture::Texture::Ptr& getDisplacementMap() const;

		/**
		 * @param displacementMap the material displacement map texture in tangent space
		 */
		void setDisplacementMap(const Texture::Texture::Ptr& displacementMap);

	};

}

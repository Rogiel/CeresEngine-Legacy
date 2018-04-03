//
// Created by Rogiel Sulzbach on 7/29/17.
//

#pragma once

#include "XYZ/Graphics/Model/Model.hpp"

namespace XYZ::Graphics::Model {

	/**
	 * A static model is a type of model that does not change frequently.
	 *
	 * A static model is optimize for static geometry, such as scenery and other objects
	 * that dont change or very rarely change.
	 */
	class StaticModel : public Model {
	private: // Mesh & Vertex Buffers
		/**
		 * The model's mesh object
		 */
		Mesh::Mesh::Ptr mesh;

		/**
		 * The model's compiled vertex buffer
		 */
		Renderer::VertexBuffer::Ptr vertexBuffer;

	private: // Phong material properties
		/**
		 * The model's diffuse color
		 */
		glm::vec3 diffuseColor = glm::vec3(0.0);

		/**
		 * The model's diffuse texture
		 */
		Texture::Texture::Ptr diffuseTexture;

		/**
		 * The model's specular color
		 */
		glm::vec3 specularColor = glm::vec3(0.0);

		/**
		 * The model's specular map texture
		 */
		Texture::Texture::Ptr specularTexture;

		/**
		 * The model's shininess
		 */
		float shininess = 32.0f;

		/**
		 * The model's normal map texture
		 */
		Texture::Texture::Ptr normalMap;

	private: // Shadows
		/**
		 * A boolean flag indicating if a model should cast shadows
		 */
		bool castShadows = true;

	public:
		/**
		 * Create a new static model with a mesh object
		 *
		 * @param mesh the model's mesh
		 * @param diffuseTexture the model's diffuse texture
		 * @param specularTexture the model's specular map texture
		 * @param shininess the model's shininess
		 * @param normalMap the model's normal map texture
		 * @param castShadows a boolean flag indicating if a model should cast shadows
		 */
		explicit StaticModel(Mesh::Mesh::Ptr mesh,
							 Texture::Texture::Ptr diffuseTexture = nullptr,
							 Texture::Texture::Ptr specularTexture = nullptr,
							 float shininess = 32.0f,
							 Texture::Texture::Ptr normalMap = nullptr,
							 bool castShadows = true);

	public:
		/**
		 * Renders the model.
		 *
		 * This method can only be called from a renderer context.
		 *
		 * @param renderer the renderer context
		 */
		void render(Renderer::Renderer& renderer, const LevelOfDetail& levelOfDetail) final;

		/**
		 * Sets the shader uniform variables for the model material
		 *
		 * @param shader the shader to set uniforms to
		 */
		void setMaterialShaderUniforms(Renderer::Renderer& renderer, Shader::ShaderProgram& shader,
									   const LevelOfDetail& levelOfDetail) final;

	public:
		virtual glm::vec3 getSize() final override;

	public:
		/**
		 * @return the model's mesh object
		 */
		const Mesh::Mesh::Ptr& getMesh() const;

		/**
		 * @param mesh the model's mesh object
		 */
		void setMesh(const Mesh::Mesh::Ptr& mesh);

		/**
		 * @return the model's compiled vertex buffer
		 */
		const Renderer::VertexBuffer::Ptr& getVertexBuffer() const;

		/**
		 * @param vertexBuffer the model's compiled vertex buffer
		 */
		void setVertexBuffer(const Renderer::VertexBuffer::Ptr& vertexBuffer);

		/**
		 * @return the model's diffuse color
		 */
		const glm::vec3& getDiffuseColor() const;

		/**
		 * @param diffuseColor the model's diffuse color
		 */
		void setDiffuseColor(const glm::vec3& diffuseColor);

		/**
		 * @return the model's diffuse texture
		 */
		const Texture::Texture::Ptr& getDiffuseTexture() const;

		/**
		 * @param diffuseTexture the model's diffuse texture
		 */
		void setDiffuseTexture(const Texture::Texture::Ptr& diffuseTexture);

		/**
		 * @return the model's specular color
		 */
		const glm::vec3& getSpecularColor() const;

		/**
		 * @param specularColor the model's specular color
		 */
		void setSpecularColor(const glm::vec3& specularColor);

		/**
		 * @return the model's specular map texture
		 */
		const Texture::Texture::Ptr& getSpecularTexture() const;

		/**
		 * @param specularTexture the model's specular map texture
		 */
		void setSpecularTexture(const Texture::Texture::Ptr& specularTexture);

		/**
		 * @return the model's shininess
		 */
		float getShininess() const;

		/**
		 * @param shininess the model's shininess
		 */
		void setShininess(float shininess);

		/**
		 * @return the model's normal map texture
		 */
		const Texture::Texture::Ptr& getNormalMap() const;

		/**
		 * @param normalMap the model's normal map texture
		 */
		void setNormalMap(const Texture::Texture::Ptr& normalMap);

		/**
		 * @return a boolean flag indicating if a model should cast shadows
		 */
		bool shouldCastShadows() const;

		/**
		 * @param castShadows a boolean flag indicating if a model should cast shadows
		 */
		void setCastShadows(bool castShadows);

	public:
		/**
		 * A event called whenever the engine is running low on memory.
		 *
		 * When calling this method, resources are able to release
		 * cached data.
		 *
		 * @return true if the entire resource can be unloaded,
		 * false otherwise.
		 */
		bool didReceiveMemoryWarning() override;

	};

}


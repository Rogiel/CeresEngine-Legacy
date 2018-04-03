//
// Created by Rogiel Sulzbach on 7/21/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Model/LevelOfDetail.hpp"

#include "XYZ/Graphics/Mesh/Mesh.hpp"
#include "XYZ/Graphics/Material/Material.hpp"

namespace XYZ::Graphics::Renderer {
	class Renderer;
}

namespace XYZ::Graphics::Model {

	class AbstractModel {

	};

	/**
	 * A model object represents a entity object that can be rendered by a renderer.
	 *
	 * A model contains generic data about a object class, that is, it encapsulates the
	 * material, the mesh, the animation data and any other data related to the object
	 * physical characteristics.
	 *
	 * The object position, rotation and scaling are not included in the model and
	 * are defined by object. This allows a model to be reused by multiple objects.
	 */
	class Model : public AbstractModel,
				  public Resource::Resource<Model> {
	public:
		/**
		 * Renders the model.
		 *
		 * This method can only be called from a renderer context.
		 *
		 * @param renderer the renderer context
		 */
		virtual void render(Renderer::Renderer& renderer, const LevelOfDetail& levelOfDetail) = 0;

		/**
		 * Sets the shader uniform variables for the model material
		 *
		 * @param shader the shader to set uniforms to
		 */
		virtual void setMaterialShaderUniforms(Renderer::Renderer& renderer, Shader::ShaderProgram& shader,
											   const LevelOfDetail& levelOfDetail) = 0;

	public:
		virtual glm::vec3 getSize() = 0;

	};

}


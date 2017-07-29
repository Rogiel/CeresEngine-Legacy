//
// Created by Rogiel Sulzbach on 7/21/17.
//

#pragma once

#include "XYZ/Scene/Scene.hpp"

#include "XYZ/Graphics/Renderer/OpenGL/OpenGLGeometryBuffer.hpp"
#include "OpenGLFramebuffer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLRenderer.hpp"
#include "OpenGLCubeMap.hpp"

#include "XYZ/Scene/Light/DirectionalLight.hpp"
#include "XYZ/Scene/Light/PointLight.hpp"
#include "XYZ/Scene/Light/SpotLight.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	class OpenGLDeferredRendering {
	private:
		OpenGLRenderer& renderer;

	private:
		/**
		 * The OpenGL geometry buffer
		 */
		OpenGLGeometryBuffer geometryBuffer;

		/**
		 * The geometry shader program
		 */
		OpenGLShaderProgram geometryBufferShader;

	private:
		/**
		 * A shadow map texture used to render directional lights shadows
		 */
		OpenGLTexture shadowMap;

		/**
		 * The OpenGL shadow framebuffer
		 */
		OpenGLFramebuffer shadowMapFBO;
		
		/**
		 * The geometry shader program
		 */
		OpenGLShaderProgram shadowMapShader;

		/**
		 * A shadow cube map texture used to render point lights shadows
		 */
		OpenGLCubeMap shadowCubeMap;

		/**
		 * The OpenGL point light shadow framebuffer
		 */
		OpenGLFramebuffer shadowCubeMapFBO;

		/**
		 * The geometry shader program
		 */
		OpenGLShaderProgram shadowCubeMapShader;

		/**
		 * The directional light shader program
		 */
		OpenGLShaderProgram directionalLightShader;

		/**
		 * The point light shader program
		 */
		OpenGLShaderProgram pointLightShader;

		/**
		 * The spot light shader program
		 */
		OpenGLShaderProgram spotLightShader;

	private:
		/**
		 * The OpenGL ViewProjection structure accessible on shaders
		 */
		OpenGLUniformBuffer<OpenGLViewProjetion> viewProjection;

	public:
		/**
		 * Creates a new deferred rendering technique
		 *
		 * @param renderer the OpenGL renderer
		 */
		explicit OpenGLDeferredRendering(OpenGLRenderer& renderer);

		/**
		 * Deleted copy constructor.
		 *
		 * @param other the instance to copy from
		 */
		OpenGLDeferredRendering(const OpenGLDeferredRendering& other) = delete;

		/**
		 * Deleted copy assignment operator.
		 *
		 * @param other the instance to copy from
		 *
		 * @return *this
		 */
		OpenGLDeferredRendering& operator=(const OpenGLDeferredRendering& other) = delete;

		/**
		 * Creates a new DeferredRendering technique by moving the contents of another
		 *
		 * @param other the instance to move from
		 */
		OpenGLDeferredRendering(OpenGLDeferredRendering&& other);

		/**
		 * Deleted move assignment operator.
		 *
		 * @param other the instance to move from
		 *
		 * @return this
		 */
		OpenGLDeferredRendering& operator=(OpenGLDeferredRendering&& other) = delete;

		/**
		 * Destroys the deferred rendering technique object
		 */
		~OpenGLDeferredRendering();

	public:
		/**
		 * Render the scene using a deferred rendering technique
		 *
		 * @param scene the scene to be rendered
		 */
		void render(Scene::Scene& scene);

		void resize(unsigned int width, unsigned height);

	private:
		/**
		 * Render the geometry buffer pass
		 */
		void renderGeometryBufferPass(Scene::Scene& scene);

		/**
		 * Render the lighting pass
		 */
		void renderLightingPass(Scene::Scene& scene);

		/**
		 * Renders the bloom pass
		 */
		void renderBloomPass();

		/**
		 * Render the HDR pass
		 */
		void renderHDRPass();

	private:
		/**
		 * Render the object given by <tt>object</tt> into the geometry buffer
		 *
		 * @param object the object to be rendered to the gbuffer
		 * @param parentModelMatrix the model matrix of the objects parent
		 */
		void renderGeometryBufferObject(Scene::Object& object, const glm::mat4& parentModelMatrix);

		glm::mat4 renderShadowMap(Scene::Scene& scene,Scene::Light::DirectionalLight& light);
		float renderShadowMap(Scene::Scene& scene,Scene::Light::PointLight& light);
		glm::mat4 renderShadowMap(Scene::Scene& scene,Scene::Light::SpotLight& light);

		void renderShadowMapObject(Scene::Object& object, OpenGLShaderProgram& shader, const glm::mat4& parentModelMatrix);
	};

	/**
	 * The Geometry Vertex shader source
	 */
	extern const Shader::ShaderSource GeometryVertexShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource GeometryFragmentShaderSource;

	// -----------------------------------------------------------------------------------------------------------------

	/**
	 * The Geometry Vertex shader source
	 */
	extern const Shader::ShaderSource ShadowCubeMapVertexShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource ShadowCubeMapFragmentShaderSource;
	
	// -----------------------------------------------------------------------------------------------------------------
	
	/**
	 * The Lighting pass vertex shader source
	 */
	extern const Shader::ShaderSource LightingVertexShaderSource;

	// -----------------------------------------------------------------------------------------------------------------
	
	/**
	 * The Directional Light shader source
	 */
	extern const Shader::ShaderSource DirectionalLightFragmentShaderSource;

	/**
	 * The Geometry Vertex shader source
	 */
	extern const Shader::ShaderSource DirectionalLightShadowMapVertexShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource DirectionalLightShadowMapFragmentShaderSource;
	
	// -----------------------------------------------------------------------------------------------------------------
	
	/**
	 * The Point Light shader source
	 */
	extern const Shader::ShaderSource PointLightFragmentShaderSource;

	/**
	 * The Geometry Vertex shader source
	 */
	extern const Shader::ShaderSource PointLightShadowMapVertexShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource PointLightShadowMapGeometryShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource PointLightShadowMapFragmentShaderSource;
	
	// -----------------------------------------------------------------------------------------------------------------
	
	/**
	 * The Spot Light shader source
	 */
	extern const Shader::ShaderSource SpotLightFragmentShaderSource;

	/**
	 * The Geometry Vertex shader source
	 */
	extern const Shader::ShaderSource SpotLightShadowMapVertexShaderSource;

	/**
	 * The Geometry Fragment shader source
	 */
	extern const Shader::ShaderSource SpotLightShadowMapFragmentShaderSource;
	
}




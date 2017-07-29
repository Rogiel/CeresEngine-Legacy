//
// Created by Rogiel Sulzbach on 7/21/17.
//

#include "OpenGLDeferredRendering.hpp"

#include "XYZ/Scene/Light/DirectionalLight.hpp"
#include "XYZ/Scene/Light/PointLight.hpp"
#include "XYZ/Scene/Light/SpotLight.hpp"

#include "OpenGLVertexBuffer.hpp"
#include "OpenGLShaderBuffers.hpp"

#include <glm/ext.hpp>

#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	enum OpenGLUniformBufferIndex : GLuint {
		VIEW_PROJECT_UNIFORM_BUFFER_INDEX = 0
	};

	OpenGLDeferredRendering::OpenGLDeferredRendering(OpenGLRenderer& renderer) :
			renderer(renderer),
			geometryBuffer(1024, 768),
			geometryBufferShader(
					OpenGLVertexShader(GeometryVertexShaderSource),
					OpenGLFragmentShader(GeometryFragmentShaderSource)
			),

			shadowMap(2048, 2048, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT),
			shadowMapFBO(2048, 2048),
			shadowMapShader(
					OpenGLVertexShader(DirectionalLightShadowMapVertexShaderSource),
					OpenGLFragmentShader(DirectionalLightShadowMapFragmentShaderSource)
			),

			shadowCubeMap(1024, 1024, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT),
			shadowCubeMapFBO(1024, 1024),
			shadowCubeMapShader(
					OpenGLVertexShader(PointLightShadowMapVertexShaderSource),
					OpenGLFragmentShader(PointLightShadowMapFragmentShaderSource)
//					, OpenGLGeometryShader(PointLightShadowMapGeometryShaderSource)
			),

			lightingFramebuffer(1024, 768),
			lightingTexture(1024, 768, GL_RGBA16F, GL_RGBA, GL_FLOAT),

			directionalLightShader(
					OpenGLVertexShader(LightingVertexShaderSource),
					OpenGLFragmentShader(DirectionalLightFragmentShaderSource)
			),
			pointLightShader(
					OpenGLVertexShader(LightingVertexShaderSource),
					OpenGLFragmentShader(PointLightFragmentShaderSource)
			),
			spotLightShader(
					OpenGLVertexShader(LightingVertexShaderSource),
					OpenGLFragmentShader(SpotLightFragmentShaderSource)
			),

			bloomHorizontalBlurFramebuffer(1024, 768),
			bloomHorizontalBlurTexture(1024, 768, GL_RGBA16F, GL_RGBA, GL_FLOAT),

			bloomVerticalBlurFramebuffer(1024, 768),
			bloomVerticalBlurTexture(1024, 768, GL_RGBA16F, GL_RGBA, GL_FLOAT),

			bloomExposureMappingShaderProgram(
					OpenGLVertexShader(BloomExposureMappingVertexShaderSource),
					OpenGLFragmentShader(BloomExposureMappingFragmentShaderSource)
			),
			bloomBlurShaderProgram(
					OpenGLVertexShader(BloomBlurVertexShaderSource),
					OpenGLFragmentShader(BloomBlurFragmentShaderSource)
			),

			hdrShaderProgram(
					OpenGLVertexShader(HDRVertexShaderSource),
					OpenGLFragmentShader(HDRFragmentShaderSource)
			) {
		viewProjection.init();

		geometryBufferShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		// -------------------------------------------------------------------------------------------------------------

		shadowMap.setWrapMode(Texture::TextureWrap::CLAMP_TO_BORDER, Texture::TextureWrap::CLAMP_TO_BORDER);
		shadowMap.setBorderColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

		shadowMapFBO.activate();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap.textureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		shadowMapFBO.deactivate();

		// -------------------------------------------------------------------------------------------------------------

		shadowCubeMap.setWrapModeR(Texture::TextureWrap::CLAMP_TO_EDGE);
		shadowCubeMap.setWrapModeS(Texture::TextureWrap::CLAMP_TO_EDGE);
		shadowCubeMap.setWrapModeT(Texture::TextureWrap::CLAMP_TO_EDGE);

		shadowCubeMapFBO.activate();
//		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMap.textureID, 0);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, shadowCubeMap.textureID, 0);
//		glDrawBuffer(GL_NONE);
//		glReadBuffer(GL_NONE);
		shadowCubeMapFBO.deactivate();

		// -----------------------------------------------------------------------------------------------------------------

		lightingTexture.setWrapMode(Texture::TextureWrap::CLAMP_TO_EDGE, Texture::TextureWrap::CLAMP_TO_EDGE);

		lightingFramebuffer.activate();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingTexture.textureID, 0);
		lightingFramebuffer.deactivate();

		// -------------------------------------------------------------------------------------------------------------

		directionalLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		directionalLightShader.activate();
		directionalLightShader.set("gPosition", 0);
		directionalLightShader.set("gNormal", 1);
		directionalLightShader.set("gAlbedoSpec", 2);
		directionalLightShader.set("shadowMap", 3);

		// -------------------------------------------------------------------------------------------------------------

		pointLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		pointLightShader.activate();
		pointLightShader.set("gPosition", 0);
		pointLightShader.set("gNormal", 1);
		pointLightShader.set("gAlbedoSpec", 2);
		pointLightShader.set("shadowMap", 3);

		// -------------------------------------------------------------------------------------------------------------

		spotLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		spotLightShader.activate();
		spotLightShader.set("gPosition", 0);
		spotLightShader.set("gNormal", 1);
		spotLightShader.set("gAlbedoSpec", 2);
		spotLightShader.set("shadowMap", 3);

		// -------------------------------------------------------------------------------------------------------------

		bloomHorizontalBlurFramebuffer.activate();
		bloomHorizontalBlurTexture.setMagnificationMinificationFilter(Texture::TextureMagnification::LINEAR,
																	Texture::TextureMinification::LINEAR);
		bloomHorizontalBlurTexture.setWrapMode(Texture::TextureWrap::CLAMP_TO_EDGE,
											 Texture::TextureWrap::CLAMP_TO_EDGE);
		glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomHorizontalBlurTexture.textureID, 0
		);

		// -------------------------------------------------------------------------------------------------------------

		bloomVerticalBlurFramebuffer.activate();
		bloomVerticalBlurTexture.setMagnificationMinificationFilter(Texture::TextureMagnification::LINEAR,
																  Texture::TextureMinification::LINEAR);
		bloomVerticalBlurTexture.setWrapMode(Texture::TextureWrap::CLAMP_TO_EDGE,
										   Texture::TextureWrap::CLAMP_TO_EDGE);
		glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomVerticalBlurTexture.textureID, 0
		);

		// -----------------------------------------------------------------------------------------------------------------

		hdrShaderProgram.activate();
		hdrShaderProgram.set("scene", 0);
		hdrShaderProgram.set("bloom", 1);
	}

	OpenGLDeferredRendering::OpenGLDeferredRendering(OpenGLDeferredRendering&& other) = default;

	OpenGLDeferredRendering::~OpenGLDeferredRendering() = default;

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::render(Scene::Scene& scene) {
		// Render the geometry pass
		renderGeometryBufferPass(scene);
		renderLightingPass(scene);
		renderBloomPass();
		renderHDRPass();
	}

	void OpenGLDeferredRendering::resize(unsigned int width, unsigned height) {
		geometryBuffer.resize(width, height);
		renderer.getDefaultFramebuffer().resize(width, height);

		lightingFramebuffer.resize(width, height);
		lightingTexture.resize(width, height);

		bloomHorizontalBlurFramebuffer.resize(width, height);
		bloomHorizontalBlurTexture.resize(width, height);

		bloomVerticalBlurFramebuffer.resize(width, height);
		bloomVerticalBlurTexture.resize(width, height);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::renderGeometryBufferPass(Scene::Scene& scene) {
		geometryBuffer.activate();
		geometryBuffer.framebuffer.faceCulling(false);

		geometryBuffer.framebuffer.clear();

		geometryBufferShader.activate();

		// Update the camera, view and projection uniform buffer
		const auto& camera = scene.getCamera();
		auto positionWithZoom = camera->getPosition() - camera->Front * camera->Zoom;

		viewProjection->camera.position = positionWithZoom;
		viewProjection->projection = glm::perspective(
				camera->getFieldOfView(), float(geometryBuffer.width) / float(geometryBuffer.height),
				camera->getZNear(), camera->getZFar());
//		float near_plane = 0.0f, far_plane = 1000.0f;
//		viewProjection->projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);

		viewProjection->view = glm::lookAt(positionWithZoom, positionWithZoom + camera->getFront(), camera->getUp());
		viewProjection.update();

		// render the root object
		renderGeometryBufferObject(*scene.getRootObject(), glm::mat4(1.0));

		geometryBufferShader.deactivate();
		geometryBuffer.deactivate();
	}

	void OpenGLDeferredRendering::renderLightingPass(Scene::Scene& scene) {
		auto& framebuffer = lightingFramebuffer;

		framebuffer.activate();
		framebuffer.clear();

		if(!lighting) {
			geometryBuffer.framebuffer.activate();
			glReadBuffer(GL_COLOR_ATTACHMENT2);
			geometryBuffer.framebuffer.copy(framebuffer);
			return;
		}

		framebuffer
				.blending(true)
				.depthTest(false)
				.faceCulling(false);
		glBlendFunc(GL_ONE, GL_ONE); // TODO expose this generically

		static unsigned int quadVAO = 0;
		static unsigned int quadVBO;

		if(quadVAO == 0) {
			float quadVertices[] = {
					// positions        // texture Coords
					-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);

		std::vector<std::shared_ptr<Scene::Light::Light>> sortedLights = scene.getLights();
		for(std::shared_ptr<Scene::Light::Light> genericLight : sortedLights) {
			switch(genericLight->getLightType()) {
				case Scene::Light::LightType::DIRECTIONAL: {
					auto light = std::static_pointer_cast<Scene::Light::DirectionalLight>(genericLight);

					directionalLightShader.activate();

					directionalLightShader.set("light.direction", light->getDirection());
					directionalLightShader.set("light.ambient", light->getAmbient());
					directionalLightShader.set("light.diffuse", light->getDiffuse());
					directionalLightShader.set("light.specular", light->getSpecular());

					break;
				}

				case Scene::Light::LightType::POINT: {
					auto light = std::static_pointer_cast<Scene::Light::PointLight>(genericLight);

					// render shadow map
					float farPlane = renderShadowMap(scene, *light);

					pointLightShader.activate();

					pointLightShader.set("light.position", light->getPosition());
					pointLightShader.set("light.ambient", light->getAmbient());
					pointLightShader.set("light.diffuse", light->getDiffuse());
					pointLightShader.set("light.specular", light->getSpecular());

					pointLightShader.set("light.constant", light->getConstant());
					pointLightShader.set("light.linear", light->getLinear());
					pointLightShader.set("light.quadratic", light->getQuadratic());

					pointLightShader.set("farPlane", farPlane);

					shadowCubeMap.activate(3);

					break;
				}

				case Scene::Light::LightType::SPOT: {
					auto light = std::static_pointer_cast<Scene::Light::SpotLight>(genericLight);

					// render shadow map
					glm::mat4 lightSpaceMatrix = renderShadowMap(scene, *light);

					spotLightShader.activate();

					spotLightShader.set("light.position", light->getPosition());
					spotLightShader.set("light.direction", light->getDirection());
					spotLightShader.set("light.cutOff", glm::cos(glm::radians(light->getCutOff())));
					spotLightShader.set("light.outerCutOff", glm::cos(glm::radians(light->getOuterCutOff())));

					spotLightShader.set("light.ambient", light->getAmbient());
					spotLightShader.set("light.diffuse", light->getDiffuse());
					spotLightShader.set("light.specular", light->getSpecular());

					spotLightShader.set("light.constant", light->getConstant());
					spotLightShader.set("light.linear", light->getLinear());
					spotLightShader.set("light.quadratic", light->getQuadratic());

					spotLightShader.set("lightSpaceMatrix", lightSpaceMatrix);

					shadowMap.activate(3);

					break;
				}
			}

			framebuffer.activate();
			framebuffer.blending(true)
					.depthTest(false)
					.faceCulling(false);
			glBlendFunc(GL_ONE, GL_ONE);

			geometryBuffer.positionDepth.activate(0);
			geometryBuffer.normalShininess.activate(1);
			geometryBuffer.albedoSpecular.activate(2);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::renderGeometryBufferObject(Scene::Object& object,
															 const glm::mat4& parentModelMatrix) {
		auto translation = object.getPosition();
		auto rotation = object.getRotation();
		auto scale = object.getScale();

		glm::mat4 modelMatrix = glm::scale(
				glm::rotate(
						glm::rotate(
								glm::rotate(
										glm::translate(
												parentModelMatrix,
												translation
										),
										rotation.z,
										glm::vec3(0.0f, 0.0f, 1.0f)
								),
								rotation.y,
								glm::vec3(0.0f, 1.0f, 0.0f)
						),
						rotation.x,
						glm::vec3(1.0f, 0.0f, 0.0f)
				),
				scale
		);

		geometryBufferShader.set("model", modelMatrix);
		geometryBufferShader.set("inversedTransposedModel", glm::transpose(glm::inverse(modelMatrix)));

		geometryBufferShader.set("material.shininess", object.getShininess());
		geometryBufferShader.set("material.diffuseColor", object.getDiffuseColor());
		geometryBufferShader.set("material.specularColor", object.getSpecularColor());

		// If there is a texture, activate it.
		if(auto diffuse = std::static_pointer_cast<OpenGLTexture>(object.getDiffuse())) {
			diffuse->activate(0);
			geometryBufferShader.set("material.diffuse", 0);
		}

		if(auto specular = std::static_pointer_cast<OpenGLTexture>(object.getSpecular())) {
			specular->activate(1);
			geometryBufferShader.set("material.specular", 1);
		}

		if(auto normalMap = std::static_pointer_cast<OpenGLTexture>(object.getNormalMap())) {
			normalMap->activate(2);
			geometryBufferShader.set("material.normal", 2);
			geometryBufferShader.set("material.hasNormalMap", true);
		} else {
			geometryBufferShader.set("material.hasNormalMap", false);
		}

		if(auto displacementMap = std::static_pointer_cast<OpenGLTexture>(object.getDisplacementMap())) {
			displacementMap->activate(3);
			geometryBufferShader.set("material.displacementMap", 3);
			geometryBufferShader.set("material.hasDisplacementMap", true);
		} else {
			geometryBufferShader.set("material.hasDisplacementMap", false);
		}

		if(const auto& mesh = object.getMesh()) {
			auto compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(object.getMesh()->getCompiledMesh());
			if(compiledMesh == nullptr) {
				// compile the mesh
				compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(renderer.getMeshCompiler().compileMesh(
						*object.getMesh()
				));
				object.getMesh()->setCompiledMesh(compiledMesh);
			}

			// Draw the triangles
			compiledMesh->draw();
		}

		// render all children
		for(const auto& child : object.getChildren()) {
			renderGeometryBufferObject(*child, modelMatrix);
		}
	}

	float OpenGLDeferredRendering::renderShadowMap(Scene::Scene& scene, Scene::Light::PointLight& light) {
		const float farPlane = 1000.0f;
		glm::mat4 lightProjection = glm::perspective<float>(glm::radians(90.0f), 1.0f, 0.1f, farPlane);

		auto lightPos = light.getPosition();

		std::vector<glm::mat4> lightSpaceMatrices(6);
		lightSpaceMatrices[0] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(1.0, 0.0, 0.0),
				glm::vec3(0.0, -1.0, 0.0)
		);
		lightSpaceMatrices[1] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(-1.0, 0.0, 0.0),
				glm::vec3(0.0, -1.0, 0.0)
		);
		lightSpaceMatrices[2] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(0.0, 1.0, 0.0),
				glm::vec3(0.0, 0.0, 1.0)
		);
		lightSpaceMatrices[3] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(0.0, -1.0, 0.0),
				glm::vec3(0.0, 0.0, -1.0)
		);
		lightSpaceMatrices[4] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(0.0, 0.0, 1.0),
				glm::vec3(0.0, -1.0, 0.0)
		);
		lightSpaceMatrices[5] = lightProjection * glm::lookAt(
				lightPos,
				lightPos + glm::vec3(0.0, 0.0, -1.0),
				glm::vec3(0.0, -1.0, 0.0)
		);

		shadowCubeMapShader.activate();
		shadowCubeMapShader.set("lightPos", lightPos);
		shadowCubeMapShader.set("farPlane", farPlane);

		for(unsigned int i = 0; i < 6; i++) {
			shadowCubeMapShader.set("lightSpaceMatrix", lightSpaceMatrices[i]);

			shadowCubeMapFBO.activate();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
								   shadowCubeMap.textureID, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glClear(GL_DEPTH_BUFFER_BIT);

			renderShadowMapObject(*scene.getRootObject(), shadowCubeMapShader, glm::mat4(1.0));
		}

		return farPlane;
	}

	glm::mat4 OpenGLDeferredRendering::renderShadowMap(Scene::Scene& scene, Scene::Light::SpotLight& light) {
		shadowMapFBO.activate();
		shadowMapFBO.clear();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection = glm::perspective<float>(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
		glm::mat4 lightView = glm::lookAt(
				light.getPosition(),
				light.getPosition() + light.getDirection(),
				glm::vec3(0, 1, 0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		shadowMapShader.activate();
		shadowMapShader.set("lightSpaceMatrix", lightSpaceMatrix);

		glCullFace(GL_FRONT);
		renderShadowMapObject(*scene.getRootObject(), shadowMapShader, glm::mat4(1.0));
		glCullFace(GL_BACK);

		return lightSpaceMatrix;
	}

	void OpenGLDeferredRendering::renderShadowMapObject(Scene::Object& object, OpenGLShaderProgram& shader,
														const glm::mat4& parentModelMatrix) {
		auto translation = object.getPosition();
		auto rotation = object.getRotation();
		auto scale = object.getScale();

		glm::mat4 modelMatrix = glm::scale(
				glm::rotate(
						glm::rotate(
								glm::rotate(
										glm::translate(
												parentModelMatrix,
												translation
										),
										rotation.z,
										glm::vec3(0.0f, 0.0f, 1.0f)
								),
								rotation.y,
								glm::vec3(0.0f, 1.0f, 0.0f)
						),
						rotation.x,
						glm::vec3(1.0f, 0.0f, 0.0f)
				),
				scale
		);

		if(object.getCastShadows()) {
			shader.set("model", modelMatrix);

			if(const auto& mesh = object.getMesh()) {
				auto compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(object.getMesh()->getCompiledMesh());
				if(compiledMesh == nullptr) {
					// compile the mesh
					compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(renderer.getMeshCompiler().compileMesh(
							*object.getMesh()
					));
					object.getMesh()->setCompiledMesh(compiledMesh);
				}

				// Draw the triangles
				compiledMesh->draw();
			}
		}

		// render all children
		for(const auto& child : object.getChildren()) {
			renderShadowMapObject(*child, shader, modelMatrix);
		}
	}

}
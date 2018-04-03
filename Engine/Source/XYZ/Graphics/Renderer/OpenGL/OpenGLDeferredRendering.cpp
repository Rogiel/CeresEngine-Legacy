//
// Created by Rogiel Sulzbach on 7/21/17.
//

#include "OpenGLDeferredRendering.hpp"

#include "XYZ/Scene/Light/DirectionalLight.hpp"
#include "XYZ/Scene/Light/PointLight.hpp"
#include "XYZ/Scene/Light/SpotLight.hpp"

#include "OpenGLVertexBuffer.hpp"
#include "OpenGLShaderBuffers.hpp"

#include "OpenGLException.hpp"

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

			shadowMap(1024, 1024, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT),
			shadowMapFBO(1024, 1024),
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
//			pointLightShader(
//					OpenGLVertexShader(LightingVertexShaderSource),
//					OpenGLFragmentShader(PointLightFragmentShaderSource)
//			),
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

//		geometryBufferShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		// -------------------------------------------------------------------------------------------------------------

		shadowMap.setWrapMode(Texture::TextureWrap::CLAMP_TO_BORDER, Texture::TextureWrap::CLAMP_TO_BORDER);
		shadowMap.setBorderColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

		shadowMapFBO.activate();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
							   shadowMap.textureID, 0);
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

		// -------------------------------------------------------------------------------------------------------------

		lightingTexture.setWrapMode(Texture::TextureWrap::CLAMP_TO_EDGE, Texture::TextureWrap::CLAMP_TO_EDGE);

		lightingFramebuffer.activate();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
							   lightingTexture.textureID, 0);
		lightingFramebuffer.deactivate();

		// -------------------------------------------------------------------------------------------------------------

//		directionalLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		directionalLightShader.activate();
		directionalLightShader.set("gPosition", 0);
		directionalLightShader.set("gNormal", 1);
		directionalLightShader.set("gAlbedoSpec", 2);
		directionalLightShader.set("shadowMap", 3);

		// -------------------------------------------------------------------------------------------------------------

//		pointLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

//		pointLightShader.activate();
//		pointLightShader.set("gPosition", 0);
//		pointLightShader.set("gNormal", 1);
//		pointLightShader.set("gAlbedoSpec", 2);
//		pointLightShader.set("shadowMap", 3);

		// -------------------------------------------------------------------------------------------------------------

//		spotLightShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

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

		// -------------------------------------------------------------------------------------------------------------

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
//		glClear(GL_DEPTH_BUFFER_BIT);

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
//		viewProjection.update();

		geometryBufferShader.set("projection", viewProjection->projection);
		geometryBufferShader.set("view", viewProjection->view);
		geometryBufferShader.set("camera.position", viewProjection->camera.position);

		auto VP = viewProjection->projection * viewProjection->view;

		// render the root object
		renderGeometryBufferObject(*scene.getRootObject(), glm::mat4(1.0), VP);

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

		unsigned int pointLightsCount = 0;

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
					directionalLightShader.set("light.shadowOcclusionStrength", light->getShadowOcclusionStrength());

					directionalLightShader.set("projection", viewProjection->projection);
					directionalLightShader.set("view", viewProjection->view);
					directionalLightShader.set("camera.position", viewProjection->camera.position);

					break;
				}

				case Scene::Light::LightType::POINT: {
					pointLightsCount++;
					break;
				}
//					auto light = std::static_pointer_cast<Scene::Light::PointLight>(genericLight);
//
//					auto distance = glm::length(scene.getCamera()->getPosition() - light->getPosition());
//
//					// render shadow map
//					float farPlane = 1000.0;
//					bool hasShadowMap = false;
//
//					if(light->hasShadows() && distance < 3.0) {
//						farPlane = renderShadowMap(scene, *light);
//						hasShadowMap = true;
//					}
//
//					pointLightShader.activate();
//
//					pointLightShader.set("light.position", light->getPosition());
//					pointLightShader.set("light.ambient", light->getAmbient());
//					pointLightShader.set("light.diffuse", light->getDiffuse());
//					pointLightShader.set("light.specular", light->getSpecular());
//
//					pointLightShader.set("light.constant", light->getConstant());
//					pointLightShader.set("light.linear", light->getLinear());
//					pointLightShader.set("light.quadratic", light->getQuadratic());
//
//					pointLightShader.set("light.shadowOcclusionStrength", light->getShadowOcclusionStrength());
//
//					pointLightShader.set("farPlane", farPlane);
//					pointLightShader.set("hasShadowMap", hasShadowMap);
//
//					pointLightShader.set("projection", viewProjection->projection);
//					pointLightShader.set("view", viewProjection->view);
//					pointLightShader.set("camera.position", viewProjection->camera.position);
//
//					shadowCubeMap.activate(3);
//
//					break;
//				}

				case Scene::Light::LightType::SPOT: {
					auto light = std::static_pointer_cast<Scene::Light::SpotLight>(genericLight);

					// render shadow map
					glm::mat4 lightSpaceMatrix;
					bool hasShadowMap = true;

					if(light->hasShadows()) {
						lightSpaceMatrix = renderShadowMap(scene, *light);
						hasShadowMap = true;
					}

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

					spotLightShader.set("light.shadowOcclusionStrength", light->getShadowOcclusionStrength());
					spotLightShader.set("lightSpaceMatrix", lightSpaceMatrix);
					spotLightShader.set("hasShadowMap", hasShadowMap);

					spotLightShader.set("projection", viewProjection->projection);
					spotLightShader.set("view", viewProjection->view);
					spotLightShader.set("camera.position", viewProjection->camera.position);

					shadowMap.activate(3);

					framebuffer.activate();
					throwOpenGLException();

					framebuffer.blending(true)
							.depthTest(false)
							.faceCulling(false);
					glBlendFunc(GL_ONE, GL_ONE);

					geometryBuffer.positionDepth.activate(0);
					geometryBuffer.normalShininess.activate(1);
					geometryBuffer.albedoSpecular.activate(2);

					glBindVertexArray(quadVAO);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					break;
				}
			}
		}
		framebuffer.deactivate();

		if(pointLightsCount == 0) {
			return;
		}

		auto found = pointLightShaders.find(pointLightsCount);
		if(found == pointLightShaders.end()) {
			auto originalSource = PointLightFragmentShaderSource.getSource();
			auto version = originalSource.substr(0, originalSource.find("\n", originalSource.find("#version")));
			auto cutSource = originalSource.substr(originalSource.find("\n", originalSource.find("#version")));

			auto source = Shader::ShaderSource(version +
											   "#define LIGHT_COUNT " + std::to_string(pointLightsCount) + "\n" +
											   cutSource);

			pointLightShaders[pointLightsCount] = OpenGLShaderProgram(
					OpenGLVertexShader(LightingVertexShaderSource),
					OpenGLFragmentShader(source)
			);
		}

		auto& pointLightShader = pointLightShaders[pointLightsCount];

		pointLightShader.activate();

		pointLightShader.set("gPosition", 0);
		pointLightShader.set("gNormal", 1);
		pointLightShader.set("gAlbedoSpec", 2);
		pointLightShader.set("shadowMap", 3);

		pointLightShader.set("projection", viewProjection->projection);
		pointLightShader.set("view", viewProjection->view);
		pointLightShader.set("camera.position", viewProjection->camera.position);

		pointLightShader.set("farPlane", 1000.0f);
		pointLightShader.set("hasShadowMap", false);

		int lightIndex = 0;
		for(std::shared_ptr<Scene::Light::Light> genericLight : sortedLights) {
			switch(genericLight->getLightType()) {
				case Scene::Light::LightType::POINT: {
					if(lightIndex == 20) {
						continue;
					}

					auto light = std::static_pointer_cast<Scene::Light::PointLight>(genericLight);

					pointLightShader.set("light[" + std::to_string(lightIndex) + "].position", light->getPosition());
					pointLightShader.set("light[" + std::to_string(lightIndex) + "].ambient", light->getAmbient());
					pointLightShader.set("light[" + std::to_string(lightIndex) + "].diffuse", light->getDiffuse());
					pointLightShader.set("light[" + std::to_string(lightIndex) + "].specular", light->getSpecular());

					pointLightShader.set("light[" + std::to_string(lightIndex) + "].constant", light->getConstant());
					pointLightShader.set("light[" + std::to_string(lightIndex) + "].linear", light->getLinear());
					pointLightShader.set("light[" + std::to_string(lightIndex) + "].quadratic", light->getQuadratic());

					pointLightShader.set("light[" + std::to_string(lightIndex) + "].shadowOcclusionStrength",
										 light->getShadowOcclusionStrength());

//					shadowCubeMap.activate(3);

					lightIndex++;

					break;
				}
			}
		}

		framebuffer.activate();
//			throwOpenGLException();

		framebuffer.blending(true)
				.depthTest(false)
				.faceCulling(false);
		glBlendFunc(GL_ONE, GL_ONE);

		geometryBuffer.positionDepth.activate(0);
		geometryBuffer.normalShininess.activate(1);
		geometryBuffer.albedoSpecular.activate(2);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::renderGeometryBufferObject(Scene::Object& object,
															 const glm::mat4& parentModelMatrix,
															 const glm::mat4& VP) {
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

		if(const auto& model = object.getModel()) {
			Model::LevelOfDetail levelOfDetail{
					glm::vec3(VP * glm::vec4(model->getSize(), 1.0))
			};

			model->setMaterialShaderUniforms(renderer, geometryBufferShader, levelOfDetail);
			model->render(renderer, levelOfDetail);
		}
//
//		if(const auto& mesh = object.getMesh()) {
//			auto compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(object.getMesh()->getCompiledMesh());
//			if(compiledMesh == nullptr) {
//				// compile the mesh
//				compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(renderer.getMeshCompiler().compileMesh(
//						*object.getMesh()
//				));
//				object.getMesh()->setCompiledMesh(compiledMesh);
//			}
//
//			// Draw the triangles
//			compiledMesh->draw();
//		}

		// render all children
		for(const auto& child : object.getChildren()) {
			renderGeometryBufferObject(*child, modelMatrix, VP);
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
//		shadowMapFBO.clear();
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

//		if(object.getModel()->sh) {

		Model::LevelOfDetail levelOfDetail{
				glm::vec3(0.0)
		};

		if(const auto& model = object.getModel()) {
			shader.set("model", modelMatrix);
			object.getModel()->render(renderer, levelOfDetail);
		}

//			if(const auto& mesh = object.getMesh()) {
//				auto compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(object.getMesh()->getCompiledMesh());
//				if(compiledMesh == nullptr) {
//					// compile the mesh
//					compiledMesh = std::static_pointer_cast<OpenGLVertexBuffer>(renderer.getMeshCompiler().compileMesh(
//							*object.getMesh()
//					));
//					object.getMesh()->setCompiledMesh(compiledMesh);
//				}
//
//				// Draw the triangles
//				compiledMesh->draw();
//			}
//		}

		// render all children
		for(const auto& child : object.getChildren()) {
			renderShadowMapObject(*child, shader, modelMatrix);
		}
	}

}
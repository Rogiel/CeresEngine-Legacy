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
			shadowMapBuffer(2048, 2048),
			shadowMapShader(
					OpenGLVertexShader(ShadowMapVertexShaderSource),
					OpenGLFragmentShader(ShadowMapFragmentShaderSource)
			),

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
			) {
		viewProjection.init();

		geometryBufferShader.set("ViewProjection", VIEW_PROJECT_UNIFORM_BUFFER_INDEX, viewProjection);

		// -------------------------------------------------------------------------------------------------------------

		shadowMap.setWrapMode(Texture::TextureWrap::CLAMP_TO_BORDER, Texture::TextureWrap::CLAMP_TO_BORDER);
		shadowMap.setBorderColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

		shadowMapBuffer.activate();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap.textureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		shadowMapBuffer.deactivate();

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
	}

	OpenGLDeferredRendering::OpenGLDeferredRendering(OpenGLDeferredRendering&& other) = default;

	OpenGLDeferredRendering::~OpenGLDeferredRendering() = default;

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::render(Scene::Scene& scene) {
		// Render the geometry pass
		renderGeometryBufferPass(scene);
		renderLightingPass(scene);
		renderHDRPass();
	}

	void OpenGLDeferredRendering::resize(unsigned int width, unsigned height) {
		geometryBuffer.resize(width, height);
		renderer.getDefaultFramebuffer().resize(width, height);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void OpenGLDeferredRendering::renderGeometryBufferPass(Scene::Scene& scene) {
		geometryBuffer.activate();
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
		auto& framebuffer = renderer.getDefaultFramebuffer();

		framebuffer.activate();
		framebuffer.clear();

		framebuffer
				.blending(true)
				.depthTest(false)
				.faceCulling(false);
		glBlendFunc(GL_ONE, GL_ONE); // TODO expose this generically

//		geometryBuffer.positionDepth.activate(0);
//		geometryBuffer.normalShininess.activate(1);
//		geometryBuffer.albedoSpecular.activate(2);
//		shadowMap.activate(3);

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
			// render shadow map
			glm::mat4 lightSpaceMatrix = renderShadowMapBuffer(scene, *genericLight);

			framebuffer.activate();
			framebuffer.blending(true)
					.depthTest(false)
					.faceCulling(false);
			glBlendFunc(GL_ONE, GL_ONE);

			geometryBuffer.positionDepth.activate(0);
			geometryBuffer.normalShininess.activate(1);
			geometryBuffer.albedoSpecular.activate(2);
			shadowMap.activate(3);

			glBindVertexArray(quadVAO);

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

					pointLightShader.activate();

					pointLightShader.set("light.position", light->getPosition());
					pointLightShader.set("light.ambient", light->getAmbient());
					pointLightShader.set("light.diffuse", light->getDiffuse());
					pointLightShader.set("light.specular", light->getSpecular());

					pointLightShader.set("light.constant", light->getConstant());
					pointLightShader.set("light.linear", light->getLinear());
					pointLightShader.set("light.quadratic", light->getQuadratic());

					break;
				}

				case Scene::Light::LightType::SPOT: {
					auto light = std::static_pointer_cast<Scene::Light::SpotLight>(genericLight);

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

					break;
				}
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
	}

	void OpenGLDeferredRendering::renderBloomPass() {

	}

	void OpenGLDeferredRendering::renderHDRPass() {
		geometryBuffer.framebuffer.copy(renderer.getDefaultFramebuffer());
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

	glm::mat4 OpenGLDeferredRendering::renderShadowMapBuffer(Scene::Scene& scene, Scene::Light::Light& light) {
		shadowMapBuffer.activate();
		shadowMapBuffer.clear();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection = glm::perspective<float>(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
		glm::mat4 lightView = glm::lookAt(
				light.getPosition(),
				light.getPosition() + static_cast<Scene::Light::SpotLight&>(light).getDirection(),
				glm::vec3(0, 1, 0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		shadowMapShader.activate();
		shadowMapShader.set("lightSpaceMatrix", lightSpaceMatrix);

		glCullFace(GL_FRONT);
		renderShadowMapBufferObject(*scene.getRootObject(), lightSpaceMatrix, glm::mat4(1.0));
		glCullFace(GL_BACK);

		return lightSpaceMatrix;
	}

	void OpenGLDeferredRendering::renderShadowMapBufferObject(Scene::Object& object, const glm::mat4& lightSpaceMatrix,
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
			shadowMapShader.set("model", modelMatrix);

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
			renderShadowMapBufferObject(*child, lightSpaceMatrix, modelMatrix);
		}
	}

}
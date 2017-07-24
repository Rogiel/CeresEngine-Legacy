//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "Light.hpp"

#include "XYZ/Graphics/Renderer/Framebuffer.hpp"

namespace XYZ::Scene::Light {

	Light::Light() {

	}

	Light::~Light() {

	}

	const glm::vec3& Light::getAmbient() const {
		return ambient;
	}

	void Light::setAmbient(const glm::vec3& ambient) {
		Light::ambient = ambient;
	}

	const glm::vec3& Light::getDiffuse() const {
		return diffuse;
	}

	void Light::setDiffuse(const glm::vec3& diffuse) {
		Light::diffuse = diffuse;
	}

	const glm::vec3& Light::getSpecular() const {
		return specular;
	}

	void Light::setSpecular(const glm::vec3& specular) {
		Light::specular = specular;
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool Light::isDirectionalLight() const {
		return getLightType() == LightType::DIRECTIONAL;
	}

	bool Light::isPointLight() const {
		return getLightType() == LightType::POINT;
	}

	bool Light::isSpotLight() const {
		return getLightType() == LightType::SPOT;
	}

	const std::shared_ptr<Renderer::Framebuffer>& Light::getShadowMapFramebuffer() const {
		return shadowMapFramebuffer;
	}

	void Light::setShadowMapFramebuffer(const std::shared_ptr<Renderer::Framebuffer>& shadowMapFramebuffer) {
		Light::shadowMapFramebuffer = shadowMapFramebuffer;
	}

	// -----------------------------------------------------------------------------------------------------------------

	float Light::getInfluenceRadius() const {
		return 0;
	}

}
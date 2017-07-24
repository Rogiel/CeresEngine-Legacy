//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "DirectionalLight.hpp"

namespace XYZ::Scene::Light {

	const glm::vec3& DirectionalLight::getDirection() const {
		return direction;
	}

	void DirectionalLight::setDirection(const glm::vec3& direction) {
		DirectionalLight::direction = direction;
	}

	// -----------------------------------------------------------------------------------------------------------------

	LightType DirectionalLight::getLightType() const {
		return LightType::DIRECTIONAL;
	}

}
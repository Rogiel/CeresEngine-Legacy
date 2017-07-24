//
// Created by Rogiel Sulzbach on 7/9/17.
//

#include "SpotLight.hpp"

namespace XYZ::Scene::Light {

	const glm::vec3& SpotLight::getDirection() const {
		return direction;
	}

	void SpotLight::setDirection(const glm::vec3& direction) {
		SpotLight::direction = direction;
	}

	float SpotLight::getCutOff() const {
		return cutOff;
	}

	void SpotLight::setCutOff(float cutOff) {
		SpotLight::cutOff = cutOff;
	}

	float SpotLight::getOuterCutOff() const {
		return outerCutOff;
	}

	void SpotLight::setOuterCutOff(float outerCutOff) {
		SpotLight::outerCutOff = outerCutOff;
	}

	float SpotLight::getConstant() const {
		return constant;
	}

	void SpotLight::setConstant(float constant) {
		SpotLight::constant = constant;
	}

	float SpotLight::getLinear() const {
		return linear;
	}

	void SpotLight::setLinear(float linear) {
		SpotLight::linear = linear;
	}

	float SpotLight::getQuadratic() const {
		return quadratic;
	}

	void SpotLight::setQuadratic(float quadratic) {
		SpotLight::quadratic = quadratic;
	}

	// -----------------------------------------------------------------------------------------------------------------

	LightType SpotLight::getLightType() const {
		return LightType::SPOT;
	}

}
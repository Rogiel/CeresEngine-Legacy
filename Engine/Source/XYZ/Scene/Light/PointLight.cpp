//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "PointLight.hpp"

namespace XYZ::Scene::Light {

	float PointLight::getConstant() const {
		return constant;
	}

	void PointLight::setConstant(float constant) {
		PointLight::constant = constant;
	}

	float PointLight::getLinear() const {
		return linear;
	}

	void PointLight::setLinear(float linear) {
		PointLight::linear = linear;
	}

	float PointLight::getQuadratic() const {
		return quadratic;
	}

	void PointLight::setQuadratic(float quadratic) {
		PointLight::quadratic = quadratic;
	}

	// -----------------------------------------------------------------------------------------------------------------

	LightType PointLight::getLightType() const {
		return LightType::POINT;
	}

	// -----------------------------------------------------------------------------------------------------------------

	float PointLight::getInfluenceRadius() const {
		float MaxChannel = fmax(fmax(getDiffuse().x, getDiffuse().y), getDiffuse().z);
		return (-linear + sqrtf(linear * linear - 4 * quadratic * (quadratic - 256 * MaxChannel * constant))) /
			   (2 * quadratic);
	}
}
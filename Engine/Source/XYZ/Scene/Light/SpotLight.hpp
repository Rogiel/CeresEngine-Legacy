//
// Created by Rogiel Sulzbach on 7/9/17.
//

#pragma once

#include "XYZ/Scene/Light/Light.hpp"

namespace XYZ::Scene::Light {

	class SpotLight : public Light {
	private:
		glm::vec3 direction;

		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

	public:
		const glm::vec3& getDirection() const;

		void setDirection(const glm::vec3& direction);

		float getCutOff() const;

		void setCutOff(float cutOff);

		float getOuterCutOff() const;

		void setOuterCutOff(float outerCutOff);

		float getConstant() const;

		void setConstant(float constant);

		float getLinear() const;

		void setLinear(float linear);

		float getQuadratic() const;

		void setQuadratic(float quadratic);

	public:
		/**
		 * @return the light type
		 */
		virtual LightType getLightType() const final override;

	};

}


//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Scene/Light/Light.hpp"

namespace XYZ::Scene::Light {

	class DirectionalLight : public Light {
	private:
		glm::vec3 direction;

	public:
		const glm::vec3& getDirection() const;
		void setDirection(const glm::vec3& direction);

	public:
		/**
		 * @return the light type
		 */
		virtual LightType getLightType() const final override;
	};

}



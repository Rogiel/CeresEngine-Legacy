//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Scene/Light/Light.hpp"

namespace XYZ::Scene::Light {

    class PointLight : public Light {
    private:
        float constant = 0.0;
        float linear = 1.0;
        float quadratic = 0.0;

    public:
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

	public:
		float getInfluenceRadius() const override;

    };

}



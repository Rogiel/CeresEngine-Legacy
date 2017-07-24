//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Scene/Object.hpp"

#include <glm/glm.hpp>

namespace XYZ::Renderer {
	class Framebuffer;
}

namespace XYZ::Scene::Light {

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};

    class Light : public Object {
    private:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

		std::shared_ptr<Renderer::Framebuffer> shadowMapFramebuffer;

	public:
        Light();
        ~Light();

    public:
        const glm::vec3& getAmbient() const;
        void setAmbient(const glm::vec3& ambient);

        const glm::vec3& getDiffuse() const;
        void setDiffuse(const glm::vec3& diffuse);

        const glm::vec3& getSpecular() const;
        void setSpecular(const glm::vec3& specular);

		const std::shared_ptr<Renderer::Framebuffer>& getShadowMapFramebuffer() const;
		void setShadowMapFramebuffer(const std::shared_ptr<Renderer::Framebuffer>& shadowMapFramebuffer);

	public:
		/**
		 * @return the light type
		 */
		virtual LightType getLightType() const = 0;

		bool isDirectionalLight() const;
		bool isPointLight() const;
		bool isSpotLight() const;

	public:
		virtual float getInfluenceRadius() const;

    };

}


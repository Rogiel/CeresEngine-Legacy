//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Scene/Object.hpp"
#include "XYZ/Scene/Light/Light.hpp"
#include "XYZ/Scene/Camera.hpp"

#include <vector>

namespace XYZ::Scene {

	class Scene {
	private:
		std::shared_ptr<Object> rootObject;
		std::vector<std::shared_ptr<Light::Light>> lights;
		std::shared_ptr<Camera> camera;

	public:
		Scene();

	public:
		const std::shared_ptr<Object>& getRootObject() const;
		void setRootObject(const std::shared_ptr<Object>& rootObject);

		const std::vector<std::shared_ptr<Light::Light>>& getLights() const;
		void addLight(const std::shared_ptr<Light::Light>& light);
		void removeLight(const std::shared_ptr<Light::Light>& light);

		const std::shared_ptr<Camera>& getCamera() const;
		void setCamera(const std::shared_ptr<Camera>& camera);


	};

}



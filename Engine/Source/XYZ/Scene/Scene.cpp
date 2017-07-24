//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "Scene.hpp"

#include "XYZ/Scene/Light/PointLight.hpp"

namespace XYZ::Scene {

    Scene::Scene() :
            lights(),
            camera(std::make_shared<Camera>()) {
    }

    const std::shared_ptr<Object> &Scene::getRootObject() const {
        return rootObject;
    }

    void Scene::setRootObject(const std::shared_ptr<Object> &rootObject) {
        Scene::rootObject = rootObject;
    }

    const std::vector<std::shared_ptr<Light::Light>>& Scene::getLights() const {
        return lights;
    }

    void Scene::addLight(const std::shared_ptr<Light::Light>& light) {
        lights.push_back(light);
    }

    void Scene::removeLight(const std::shared_ptr<Light::Light>& light) {
        auto found = std::find(lights.begin(), lights.end(), light);
        if(found == lights.end()) {
            return;
        }
        lights.erase(found);
    }

    const std::shared_ptr<Camera> &Scene::getCamera() const {
        return camera;
    }

    void Scene::setCamera(const std::shared_ptr<Camera> &camera) {
        Scene::camera = camera;
    }

}

//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "Camera.hpp"

namespace XYZ::Scene {
//
//    Camera::Camera() {
//        setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
//    }

    float Camera::getFieldOfView() const {
        return fieldOfView;
    }

    void Camera::setFieldOfView(float fieldOfView) {
        Camera::fieldOfView = fieldOfView;
    }

    float Camera::getAspectRatio() const {
        return aspectRatio;
    }

    void Camera::setAspectRatio(float aspectRatio) {
        Camera::aspectRatio = aspectRatio;
    }

    float Camera::getZNear() const {
        return zNear;
    }

    void Camera::setZNear(float zNear) {
        Camera::zNear = zNear;
    }

    float Camera::getZFar() const {
        return zFar;
    }

    void Camera::setZFar(float zFar) {
        Camera::zFar = zFar;
    }

    const glm::vec3& Camera::getFront() const {
        return Front;
    }

    const glm::vec3& Camera::getUp() const {
        return Up;
    }

    const glm::vec3& Camera::getRight() const {
        return Right;
    }

    const glm::vec3& Camera::getWorldUp() const {
        return WorldUp;
    }

}

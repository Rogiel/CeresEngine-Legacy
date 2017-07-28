//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "XYZ/Scene/Object.hpp"

namespace XYZ::Scene {

    class Camera : public Object {
//    public:
//        virtual glm::vec3 getPosition();
//
//        virtual glm::vec3 getFront();
//        virtual glm::vec3 getBack();
//
//        virtual glm::vec3 getRight();
//        virtual glm::vec3 getLeft();

    public:
        float fieldOfView = 45.0f;
        float aspectRatio = 4.0f / 3.0f;
        float zNear = 0.1;
        float zFar = 1000.0;

        // Camera Attributes
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // Eular Angles
        float Yaw;
        float Pitch;
        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

    public:
        // Default camera values
        static constexpr const float YAW        = -90.0f;
        static constexpr const float PITCH      =  0.0f;
        static constexpr const float SPEED      =  2.5f;
        static constexpr const float SENSITIVTY =  0.1f;
        static constexpr const float ZOOM       =  0.0f;

        enum Camera_Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };

    public:
        float getFieldOfView() const;
        void setFieldOfView(float fieldOfView);
        float getAspectRatio() const;
        void setAspectRatio(float aspectRatio);
        float getZNear() const;
        void setZNear(float zNear);
        float getZFar() const;
        void setZFar(float zFar);

        const glm::vec3& getFront() const;
        const glm::vec3& getUp() const;
        const glm::vec3& getRight() const;
        const glm::vec3& getWorldUp() const;

    public:
        // Constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
        {
            position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }
        // Constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
        {
            position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + Front, Up);
        }

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                position += Front * velocity;
            if (direction == BACKWARD)
                position -= Front * velocity;
            if (direction == LEFT)
                position -= Right * velocity;
            if (direction == RIGHT)
                position += Right * velocity;

            updateCameraVectors();
        }

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw   += xoffset;
            Pitch += yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // Update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset)
        {
            if (fieldOfView >= 1.0f && fieldOfView <= 45.0f)
                fieldOfView -= yoffset;
            if (fieldOfView <= 1.0f)
                fieldOfView = 1.0f;
            if (fieldOfView >= 45.0f)
                fieldOfView = 45.0f;
        }

    public:
        // Calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors() {
            // Calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            // Also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up    = glm::normalize(glm::cross(Right, Front));
        }

    };

}


//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Graphics/Model/Model.hpp"

#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace XYZ::Scene {

    class Object : public std::enable_shared_from_this<Object> {
    public:
        using Ptr = std::shared_ptr<Object>;

    public:
        using Position = glm::vec3;
        using Rotation = glm::vec3;
        using Scale = glm::vec3;

    private:
        std::weak_ptr<Object> parent;
        std::vector<std::shared_ptr<Object>> children;

    public:
        Position position = Scale(0.0f);
        Rotation rotation = Scale(0.0f);
        Scale scale = Scale(1.0f);

    private:
        Graphics::Model::Model::Ptr model;

    public:
        Object();
        virtual ~Object();

    public:
        std::shared_ptr<Object> getParent() const;
        void setParent(const std::shared_ptr<Object> &parent);

        const std::vector<std::shared_ptr<Object>> &getChildren() const;
        void addChild(const std::shared_ptr<Object> &child);
        void removeChild(const std::shared_ptr<Object> &child);
        std::shared_ptr<Object> createChild();

    public:
        Position getPosition() const;
        void setPosition(Position position);

        Rotation getRotation() const;
        void setRotation(Rotation rotation);

        const Scale getScale() const;
        void setScale(const Scale &scale);

    public:
        float x();
        void x(float x);

        float y();
        void y(float y);

        float z();
        void z(float z);

    public:
        float rotationX();
        void rotationX(float x);

        float rotationY();
        void rotationY(float y);

        float rotationZ();
        void rotationZ(float z);

    public:
        float scaleX();
        void scaleX(float x);

        float scaleY();
        void scaleY(float y);

        float scaleZ();
        void scaleZ(float z);


    public:
        const Graphics::Model::Model::Ptr& getModel() const;
        void setModel(const Graphics::Model::Model::Ptr& model);

    };

}


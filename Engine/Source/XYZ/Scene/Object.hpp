//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include "XYZ/Graphics/Mesh/Mesh.hpp"
#include "XYZ/Graphics/Shader/ShaderProgram.hpp"

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
        Graphics::Mesh::Mesh::Ptr mesh;
        XYZ::Graphics::Shader::ShaderProgram::Ptr shader;

        glm::vec3 diffuseColor = glm::vec3(0.0);
        XYZ::Graphics::Texture::Texture::Ptr diffuse;

        glm::vec3 specularColor = glm::vec3(0.0);
        XYZ::Graphics::Texture::Texture::Ptr specular;

        float shininess = 32.0f;

        XYZ::Graphics::Texture::Texture::Ptr normalMap;
        XYZ::Graphics::Texture::Texture::Ptr displacementMap;

        bool castShadows = true;

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
        const Graphics::Mesh::Mesh::Ptr &getMesh() const;
        void setMesh(const Graphics::Mesh::Mesh::Ptr &mesh);

        const XYZ::Graphics::Shader::ShaderProgram::Ptr &getShader() const;
        void setShader(const XYZ::Graphics::Shader::ShaderProgram::Ptr &shader);

//        const Graphics::Texture::Texture::Ptr& getTexture() const;
//        void setTexture(const Graphics::Texture::Texture::Ptr& texture);

        const Graphics::Texture::Texture::Ptr& getDiffuse() const;
        void setDiffuse(const std::shared_ptr <Graphics::Texture::Texture>& diffuse);

        const std::shared_ptr <Graphics::Texture::Texture>& getSpecular() const;
        void setSpecular(const std::shared_ptr <Graphics::Texture::Texture>& specular);

        const glm::vec3& getDiffuseColor() const;
        void setDiffuseColor(const glm::vec3& diffuseColor);

        const glm::vec3& getSpecularColor() const;
        void setSpecularColor(const glm::vec3& specularColor);

        float getShininess() const;
        void setShininess(float shininess);

        const Graphics::Texture::Texture::Ptr& getNormalMap() const;
        void setNormalMap(const Graphics::Texture::Texture::Ptr& normalMap);

        const Graphics::Texture::Texture::Ptr& getDisplacementMap() const;
        void setDisplacementMap(const Graphics::Texture::Texture::Ptr& displacementMap);

        bool getCastShadows() const;

        void setCastShadows(bool castShadows);

    };

}


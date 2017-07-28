//
// Created by Rogiel Sulzbach on 7/4/17.
//

#include "XYZ/Graphics/Shader/ShaderProgram.hpp"
#include "Object.hpp"

namespace XYZ::Scene {

    Object::Object() { }

    Object::~Object() { }

    // -----------------------------------------------------------------------------------------------------------------

    std::shared_ptr<Object> Object::getParent() const {
        return parent.lock();
    }

    void Object::setParent(const std::shared_ptr<Object> &parent) {
        if(auto p = Object::parent.lock()) {
            p->removeChild(shared_from_this());
        }
        if(parent == nullptr) {
            Object::parent.reset();
            return;
        }
        parent->addChild(shared_from_this());
    }

    const std::vector<std::shared_ptr<Object>> &Object::getChildren() const {
        return children;
    }

    void Object::addChild(const std::shared_ptr<Object> &child) {
        auto found = std::find(children.begin(), children.end(), child);
        if(found != children.end()) {
            return;
        }
        child->parent = shared_from_this();
        children.push_back(child);
    }

    void Object::removeChild(const std::shared_ptr<Object> &child) {
        auto found = std::find(children.begin(), children.end(), child);
        if(found == children.end()) {
            return;
        }
        children.erase(found);
        child->parent.reset();
    }

    std::shared_ptr<Object> Object::createChild() {
        auto child = std::make_shared<Object>();
        addChild(child);
        return child;
    }

    // -----------------------------------------------------------------------------------------------------------------

    Object::Position Object::getPosition() const {
        return position;
    }

    void Object::setPosition(Object::Position position) {
        Object::position = position;
    }

    Object::Rotation Object::getRotation() const {
        return rotation;
    }

    void Object::setRotation(Object::Rotation rotation) {
        Object::rotation = rotation;
    }

    const Object::Scale Object::getScale() const {
        return scale;
    }

    void Object::setScale(const Object::Scale &scale) {
        Object::scale = scale;
    }

    // -----------------------------------------------------------------------------------------------------------------

    float Object::x() {
        return position.x;
    }

    void Object::x(float x) {
        position.x = x;
    }

    float Object::y() {
        return position.y;
    }

    void Object::y(float y) {
        position.y = y;
    }

    float Object::z() {
        return position.z;
    }

    void Object::z(float z) {
        position.z = z;
    }

    float Object::rotationX() {
        return rotation.x;
    }

    void Object::rotationX(float x) {
        rotation.x = x;
    }

    float Object::rotationY() {
        return rotation.y;
    }

    void Object::rotationY(float y) {
        rotation.y = y;
    }

    float Object::rotationZ() {
        return rotation.z;
    }

    void Object::rotationZ(float z) {
        rotation.z = z;
    }

    float Object::scaleX() {
        return scale.x;
    }

    void Object::scaleX(float x) {
        scale.x = x;
    }

    float Object::scaleY() {
        return scale.y;
    }

    void Object::scaleY(float y) {
        scale.y = y;
    }

    float Object::scaleZ() {
        return scale.z;
    }

    void Object::scaleZ(float z) {
        scale.z = z;
    }

    // -----------------------------------------------------------------------------------------------------------------

    const XYZ::Graphics::Mesh::Mesh::Ptr &Object::getMesh() const {
        return mesh;
    }

    void Object::setMesh(const XYZ::Graphics::Mesh::Mesh::Ptr &mesh) {
        Object::mesh = mesh;
    }

    const XYZ::Graphics::Shader::ShaderProgram::Ptr &Object::getShader() const {
        return shader;
    }

    void Object::setShader(const XYZ::Graphics::Shader::ShaderProgram::Ptr &shader) {
        Object::shader = shader;
    }

    const Graphics::Texture::Texture::Ptr& Object::getDiffuse() const {
        return diffuse;
    }

    void Object::setDiffuse(const Graphics::Texture::Texture::Ptr& diffuse) {
        Object::diffuse = diffuse;
    }

    const Graphics::Texture::Texture::Ptr& Object::getSpecular() const {
        return specular;
    }

    void Object::setSpecular(const Graphics::Texture::Texture::Ptr& specular) {
        Object::specular = specular;
    }

    const glm::vec3& Object::getDiffuseColor() const {
        return diffuseColor;
    }

    void Object::setDiffuseColor(const glm::vec3& diffuseColor) {
        Object::diffuseColor = diffuseColor;
    }

    const glm::vec3& Object::getSpecularColor() const {
        return specularColor;
    }

    void Object::setSpecularColor(const glm::vec3& specularColor) {
        Object::specularColor = specularColor;
    }

    float Object::getShininess() const {
        return shininess;
    }

    void Object::setShininess(float shininess) {
        Object::shininess = shininess;
    }

    const Graphics::Texture::Texture::Ptr& Object::getNormalMap() const {
        return normalMap;
    }

    void Object::setNormalMap(const Graphics::Texture::Texture::Ptr& normalMap) {
        Object::normalMap = normalMap;
    }

    const Graphics::Texture::Texture::Ptr& Object::getDisplacementMap() const {
        return displacementMap;
    }

    void Object::setDisplacementMap(const Graphics::Texture::Texture::Ptr& displacementMap) {
        Object::displacementMap = displacementMap;
    }

	bool Object::getCastShadows() const {
		return castShadows;
	}

	void Object::setCastShadows(bool castShadows) {
		Object::castShadows = castShadows;
	}

//    const Graphics::Texture::Texture::Ptr& Object::getTexture() const {
//        return texture;
//    }
//
//    void Object::setTexture(const Graphics::Texture::Texture::Ptr& texture) {
//        Object::texture = texture;
//    }

}
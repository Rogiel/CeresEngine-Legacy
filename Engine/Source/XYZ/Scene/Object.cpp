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

    const Graphics::Model::Model::Ptr& Object::getModel() const {
        return model;
    }

    void Object::setModel(const Graphics::Model::Model::Ptr& model) {
        Object::model = model;
    }

}
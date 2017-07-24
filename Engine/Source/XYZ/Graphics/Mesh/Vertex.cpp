//
// Created by Rogiel Sulzbach on 7/22/17.
//

#include "Vertex.hpp"

namespace XYZ::Graphics::Mesh {

	Vertex::Vertex() = default;

	Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords,
				   const glm::vec3& tangent) :
			position(position), normal(normal),
			texCoords(texCoords), tangent(tangent) {}

	// -----------------------------------------------------------------------------------------------------------------

	const glm::vec3& Vertex::getPosition() const {
		return position;
	}

	void Vertex::setPosition(const glm::vec3& position) {
		Vertex::position = position;
	}

	const glm::vec3& Vertex::getNormal() const {
		return normal;
	}

	void Vertex::setNormal(const glm::vec3& normal) {
		Vertex::normal = normal;
	}

	const glm::vec2& Vertex::getTexCoords() const {
		return texCoords;
	}

	void Vertex::setTexCoords(const glm::vec2& texCoords) {
		Vertex::texCoords = texCoords;
	}

	const glm::vec3& Vertex::getTangent() const {
		return tangent;
	}

	void Vertex::setTangent(const glm::vec3& tangent) {
		Vertex::tangent = tangent;
	}

}
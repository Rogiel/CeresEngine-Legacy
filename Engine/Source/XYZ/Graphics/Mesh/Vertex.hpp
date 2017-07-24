//
// Created by Rogiel Sulzbach on 7/22/17.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace XYZ::Graphics::Mesh {

	class Vertex {
	public: // attributes are public so that we can access them directly
		/**
		 * The vertex position
		 */
		glm::vec3 position;

		/**
		 * The vertex normal vector
		 */
		glm::vec3 normal;

		/**
		 * The vertex texture coordinates
		 */
		glm::vec2 texCoords;

		/**
		 * The vertex tangent vector
		 */
		glm::vec3 tangent;

	public:
		/**
		 * Create a new empty vertex
		 */
		Vertex();

		/**
		 * Create a new vertex
		 *
		 * @param position 	the vertex position
		 * @param normal 	the vertex normal vector
		 * @param texCoords the vertex texture coordinates
		 * @param tangent 	the vertex tangent vector
		 */
		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords,
			   const glm::vec3& tangent);

	public:
		/**
		 * @return the vertex position
		 */
		const glm::vec3& getPosition() const;

		/**
		 * @param position the vertex position
		 */
		void setPosition(const glm::vec3& position);

		/**
		 * @return the vertex normal vector
		 */
		const glm::vec3& getNormal() const;

		/**
		 * @param normal the vertex normal vector
		 */
		void setNormal(const glm::vec3& normal);

		/**
		 * @return the vertex texture coordinates
		 */
		const glm::vec2& getTexCoords() const;

		/**
		 * @param texCoords the vertex texture coordinates
		 */
		void setTexCoords(const glm::vec2& texCoords);

		/**
		 * @return the vertex tangent vector
		 */
		const glm::vec3& getTangent() const;

		/**
		 * @param tangent the vertex tangent vector
		 */
		void setTangent(const glm::vec3& tangent);

	};

}




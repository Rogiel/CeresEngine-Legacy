//
// Created by Rogiel Sulzbach on 6/28/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"
#include "XYZ/Graphics/Renderer/VertexBuffer.hpp"

#include "XYZ/Graphics/Mesh/Vertex.hpp"

#include <vector>
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace XYZ::Graphics::Mesh {
	class Mesh : public Resource::Resource<Mesh> {
	public:
		using Index = unsigned int;

	private:
		/**
		 * The mesh triangle vertices indices
		 */
		std::vector<unsigned int> indices;

		/**
		 * The mesh vertices
		 */
		std::vector<Vertex> vertices;

	private:
		/**
		 * A reference to a compiled mesh
		 */
		std::shared_ptr<Renderer::VertexBuffer> compiledMesh;

	public:
		/**
		 * Creates a new mesh
		 *
		 * @param indices the mesh triangle vertices indices
		 * @param vertices the mesh vertices
		 */
		Mesh(std::vector<unsigned int> indices,
			 std::vector<Vertex> vertices);

		/**
		 * Destroys the mesh.
		 */
		virtual ~Mesh() = default;

	public:
		/**
		 * @return the mesh triangle vertices indices
		 */
		const std::vector<unsigned int>& getIndices() const;

		/**
		 * @param indices the mesh triangle vertices indices
		 */
		void setIndices(const std::vector<unsigned int>& indices);

		/**
		 * @return the mesh vertices
		 */
		const std::vector<Vertex>& getVertices() const;

		/**
		 * @param vertices the mesh vertices
		 */
		void setVertices(const std::vector<Vertex>& vertices);

	public:
		/**
		 * @return the total number of vertices in the mesh
		 */
		unsigned int getVertexCount() const;

		/**
		 * Gets a vertex by its index
		 *
		 * @param index the vertex index
		 *
		 * @return the vertex at the given index
		 */
		const Vertex& getVertex(unsigned int index) const;

		/**
		 * Gets a vertex by its index
		 *
		 * @param index the vertex index
		 *
		 * @return the vertex at the given index
		 */
		Vertex& getVertex(unsigned int index);

		/**
		 * Gets a vertex by its index
		 *
		 * @param index the vertex index
		 *
		 * @return the vertex at the given index
		 */
		const Vertex& operator[](unsigned int index) const;

		/**
		 * Gets a vertex by its index
		 *
		 * @param index the vertex index
		 *
		 * @return the vertex at the given index
		 */
		Vertex& operator[](unsigned int index);

	public:
		/**
		 * @return the total number of triangles in the mesh
		 */
		unsigned int getTriangleCount() const;

		/**
		 * Gets a triangle by its index
		 *
		 * @param index the triangle index
		 *
		 * @return the 3 vertices composing the triangle
		 */
		std::array<Vertex, 3> getTriangle(unsigned int index) const;

		/**
		 * The mesh bounding box
		 *
		 * @return the mesh bounding box
		 */
		std::array<glm::vec3, 4> getBoundingBox() const;

	public:
		/**
		 * @return a reference to a compiled mesh
		 */
		const std::shared_ptr<Renderer::VertexBuffer>& getCompiledMesh() const;

		/**
		 * @param compiledMesh a reference to a compiled mesh
		 */
		void setCompiledMesh(const std::shared_ptr<Renderer::VertexBuffer>& compiledMesh);

	};
}



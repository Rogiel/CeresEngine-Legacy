//
// Created by Rogiel Sulzbach on 6/28/17.
//

#include "Mesh.hpp"

#include <glm/glm.hpp>
#include <glm/common.hpp>

namespace XYZ::Graphics::Mesh {

	Mesh::Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices) :
			indices(std::move(indices)), vertices(std::move(vertices)) {
		assert((this->indices.size() % 3) == 0);
	}

	const std::vector<unsigned int>& Mesh::getIndices() const {
		return indices;
	}

	void Mesh::setIndices(const std::vector<unsigned int>& indices) {
		Mesh::indices = indices;
	}

	const std::vector<Vertex>& Mesh::getVertices() const {
		return vertices;
	}

	void Mesh::setVertices(const std::vector<Vertex>& vertices) {
		Mesh::vertices = vertices;
	}

	// -----------------------------------------------------------------------------------------------------------------

	unsigned int Mesh::getVertexCount() const {
		return static_cast<unsigned int>(vertices.size());
	}

	const Vertex& Mesh::getVertex(unsigned int index) const {
		return vertices[index];
	}

	Vertex& Mesh::getVertex(unsigned int index) {
		return vertices[index];
	}

	const Vertex& Mesh::operator[](unsigned int index) const {
		return getVertex(index);
	}

	Vertex& Mesh::operator[](unsigned int index) {
		return getVertex(index);
	}

	// -----------------------------------------------------------------------------------------------------------------

	unsigned int Mesh::getTriangleCount() const {
		return static_cast<unsigned int>(indices.size() / 3);
	}

	std::array<Vertex, 3> Mesh::getTriangle(unsigned int index) const {
		return std::array<Vertex, 3>{
				getVertex(indices[3 * index + 0]),
				getVertex(indices[3 * index + 1]),
				getVertex(indices[3 * index + 2])
		};
	}

	// -----------------------------------------------------------------------------------------------------------------

	const std::shared_ptr<Renderer::VertexBuffer>& Mesh::getCompiledMesh() const {
		return compiledMesh;
	}

	void Mesh::setCompiledMesh(const std::shared_ptr<Renderer::VertexBuffer>& compiledMesh) {
		Mesh::compiledMesh = compiledMesh;
	}

	// -----------------------------------------------------------------------------------------------------------------

//	Mesh::Ptr Mesh::sphere(uint32_t stacks, uint32_t slices) {
//		std::vector<Mesh::Vertex> vertices(stacks * slices);
//
//		// Calc The Vertices
//		for(int i = 0; i <= stacks; ++i) {
//
//			float V = i / (float) stacks;
//			float phi = V * (float) M_PI;
//
//			// Loop Through Slices
//			for(int j = 0; j <= slices; ++j) {
//
//				float U = j / (float) slices;
//				float theta = U * ((float) M_PI * 2);
//
//				// Calc The Vertex Positions
//				float x = cosf(theta) * sinf(phi);
//				float y = cosf(phi);
//				float z = sinf(theta) * sinf(phi);
//
//				// Push Back Vertex Data
//				vertices.push_back(Mesh::Vertex(x, y, z) * 1.0f);
//			}
//		}
//
//		return std::make_shared<Mesh>(
//				vertices, std::vector<Mesh::UV>{}, std::vector<Mesh::Normal>{}
//		);
//	}
//
//	Mesh::Ptr Mesh::plane() {
//		return std::make_shared<Mesh>(
//				std::vector<Vertex>{ /* vertices */
//						{10.0f,  -0.5f, 10.0f},
//						{-10.0f, -0.5f, 10.0f},
//						{-10.0f, -0.5f, -10.0f},
//
//						{10.0f,  -0.5f, 10.0f},
//						{-10.0f, -0.5f, -10.0f},
//						{10.0f,  -0.5f, -10.0f},
//				}, std::vector<UV>{ /* uvs */
//						{10.0f, 0.0f},
//						{0.0f,  0.0f},
//						{0.0f,  10.0f},
//
//						{10.0f, 0.0f},
//						{0.0f,  10.0f},
//						{10.0f, 10.0f}
//				}, std::vector<Normal>{ /* normals */
//						{0.0f, 1.0f, 0.0f},
//						{0.0f, 1.0f, 0.0f},
//						{0.0f, 1.0f, 0.0f},
//						{0.0f, 1.0f, 0.0f},
//
//						{0.0f, 1.0f, 0.0f},
//						{0.0f, 1.0f, 0.0f},
//						{0.0f, 1.0f, 0.0f},
//				}
//		);
//	}
//
//	Mesh::Ptr Mesh::cube() {
//		float v[] = {
//				// positions          // normals           // texture coords
//				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
//				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
//				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
//				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
//				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//
//				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//
//				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//				-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//				-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//
//				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//
//				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
//				0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
//				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//				-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
//
//				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
//		};
//
//		std::vector<Vertex> vertices;
//		std::vector<UV> uvs;
//		std::vector<Normal> normals;
//		for(int i = 0; i < sizeof(v); i = i + 8) {
//			vertices.emplace_back(v[i], v[i + 1], v[i + 2]);
//			normals.emplace_back(v[i + 3], v[i + 4], v[i + 5]);
//			uvs.emplace_back(v[i + 6], v[i + 7]);
//		}
//
//		return std::make_shared<Mesh>(vertices, uvs, normals);
//	}

}
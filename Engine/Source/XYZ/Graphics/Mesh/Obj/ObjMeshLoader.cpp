//
// Created by Rogiel Sulzbach on 7/2/17.
//

#include "ObjMeshLoader.hpp"

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

namespace XYZ::Graphics::Mesh::Obj {

	bool ObjMeshLoader::supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) {
		return true;
	}

	Mesh::Ptr ObjMeshLoader::load(std::unique_ptr<Resource::Locator::ResourceStream> resourceStream) {
		std::vector<unsigned int> indices;
		std::vector<Vertex> vertices;

		Resource::Locator::ResourceStreamBuf streambuf(std::move(resourceStream));
		std::istream is(&streambuf);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &is);

		assert(shapes.size() == 1);
		assert(attrib.vertices.size() % 3 == 0);

		auto& mesh = shapes[0].mesh;

		unsigned int indexOffset = 0;
		for(size_t fv : mesh.num_face_vertices) {

			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);

			for(size_t v = 0; v < fv; v++) {
				Vertex vertex;

				// access to vertex
				tinyobj::index_t idx = mesh.indices[indexOffset + v];
				vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];

				vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];

				if(!attrib.texcoords.empty()) {
					vertex.texCoords.x = attrib.texcoords[2 * idx.texcoord_index + 0];
					vertex.texCoords.y = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				vertices.push_back(vertex);
			}

			glm::vec3 vertex0 = vertices.at(vertices.size() - 3).position;
			glm::vec3 vertex1 = vertices.at(vertices.size() - 2).position;
			glm::vec3 vertex2 = vertices.at(vertices.size() - 1).position;

			glm::vec3 normal = glm::cross((vertex1 - vertex0), (vertex2 - vertex0));
			glm::vec3 deltaPos;
			if(vertex0 == vertex1) {
				deltaPos = vertex2 - vertex0;
			} else {
				deltaPos = vertex1 - vertex0;
			}

			glm::vec2 uv0 = vertices.at(vertices.size() - 3).texCoords;
			glm::vec2 uv1 = vertices.at(vertices.size() - 2).texCoords;
			glm::vec2 uv2 = vertices.at(vertices.size() - 1).texCoords;

			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			glm::vec3 tan; // tangents
			// avoid divion with 0
			if(deltaUV1.s != 0) {
				tan = deltaPos / deltaUV1.s;
			} else {
				tan = deltaPos / 1.0f;
			}

			tan = glm::normalize(tan - glm::dot(normal, tan) * normal);

			// write into array - for each vertex of the face the same value
			vertices[vertices.size() - 3].tangent = tan;
			vertices[vertices.size() - 2].tangent = tan;
			vertices[vertices.size() - 1].tangent = tan;

			indexOffset += fv;
		}

		return std::make_shared<Mesh>(
				indices, vertices
		);
	}


}
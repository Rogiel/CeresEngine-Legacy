//
// Created by Rogiel Sulzbach on 10/8/17.
//

#include "tiny_obj_loader.h"
#include <vector>
#include <fstream>

extern "C" {
#include "ObjLoader.h"

struct OBJHandle {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
};

OBJMesh* OBJLoad(const char* path) {
	OBJHandle* handle = new OBJHandle();

	std::ifstream is(path);
	std::string err;
	tinyobj::LoadObj(&handle->attrib, &handle->shapes, &handle->materials, &err, &is);

	OBJMesh* obj = new OBJMesh();
	obj->handle = handle;

	auto& mesh = handle->shapes[0].mesh;
	OBJVertex* vertices = new OBJVertex[mesh.num_face_vertices.size() * 3];
	obj->vertices = vertices;
	obj->count = mesh.num_face_vertices.size() * 3;

	auto& attrib = handle->attrib;

	unsigned int indexOffset = 0;
	for(unsigned int fv : mesh.num_face_vertices) {
		for(unsigned int v = 0; v < fv; v++) {
			tinyobj::index_t idx = mesh.indices[indexOffset + v];
			auto& vertex = vertices[indexOffset + v];

			vertex.position[0] = attrib.vertices[3 * idx.vertex_index + 0];
			vertex.position[1] = attrib.vertices[3 * idx.vertex_index + 1];
			vertex.position[2] = attrib.vertices[3 * idx.vertex_index + 2];

			vertex.normal[0] = attrib.normals[3 * idx.normal_index + 0];
			vertex.normal[1] = attrib.normals[3 * idx.normal_index + 1];
			vertex.normal[2] = attrib.normals[3 * idx.normal_index + 2];

			if(!attrib.texcoords.empty()) {
				vertex.texCoords[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertex.texCoords[1] = attrib.texcoords[2 * idx.texcoord_index + 1];
			}
		}

		indexOffset += fv;
	}

	return obj;
}

void OBJRelease(OBJMesh* mesh) {
	delete mesh->handle;
	delete mesh;
}

}
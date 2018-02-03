//
// Created by Rogiel Sulzbach on 10/8/17.
//

#include <stdint.h>
#include <stdlib.h>

struct OBJVertex {
	float position[3];
	float normal[3];
	float texCoords[2];
};

struct OBJHandle;

struct OBJMesh {
	struct OBJVertex* vertices;
	size_t count;

	struct OBJHandle* handle;
};

struct OBJMesh* OBJLoad(const char* path);

void OBJRelease(struct OBJMesh* mesh);

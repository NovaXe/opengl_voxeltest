#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <cglm/cglm.h>
#include "chunk.h"

typedef struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}Vertex;


typedef struct ChunkMesh {
	unsigned int VAO;
	unsigned int VBO;

	Vertex vertices[4096];

}ChunkMesh;



#endif

#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <cglm/cglm.h>
#include <stdbool.h>

#define BUFFER_SIZE 512000



typedef enum mesh_vertexAttribs {
	vertexAttribPos = 0,
	vertexAttribNorm = 1,
	vertexAttribTex = 2,
	vertexAttribColor = 3

}mesh_vertexAttribs;


typedef struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texcoord;
	vec3 color;
}Vertex;


typedef struct ChunkMesh {
	unsigned int VAO;
	unsigned int VBO;
	int vertexCount;
	bool needsRegen;

	Vertex vertices[BUFFER_SIZE];

}ChunkMesh;


extern ChunkMesh* chunkmesh_meshes;
extern bool render_wireframe;

void chunkmesh_allocate();

void chunkmesh_init(ChunkMesh* mesh);

void chunkmesh_update(ChunkMesh* mesh);

void chunkmesh_draw(ChunkMesh* mesh, vec3 chunkPos);



#endif

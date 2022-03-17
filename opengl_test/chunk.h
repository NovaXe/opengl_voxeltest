#ifndef CHUNK_H
#define CHUNK_H

#include <cglm/cglm.h>
#include "chunkmesh.h"
#include "block.h"



#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Z 16

#define CHUNK_SIZE (CHUNK_SIZE_Y * CHUNK_SIZE_X * CHUNK_SIZE_Z)



typedef struct Chunk {
	BlockMutation_t* grid[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	ChunkMesh* mesh;
	int xPos;
	int yPos;
	int zPos;

	vec3 blockPos;

}Chunk;


void chunk_initialize(int chunkX, int chunkY, int chunkZ);

void chunk_populate(Chunk* chunk);

void chunk_generateMesh(Chunk* chunk);

void chunk_addCube(Chunk* chunk, ChunkMesh* mesh, BlockMutation_t* block, vec3 blockPos);

void chunk_blockOcclusion(Chunk* chunk, vec3 blockPos, bool* faces);


void chunk_checkNull(Chunk* chunk);

#endif
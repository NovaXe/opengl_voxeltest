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
	BlockMutation* grid[CHUNK_SIZE_Y][CHUNK_SIZE_X][CHUNK_SIZE_Z];
	ChunkMesh mesh;
	int yPos;
	int xPos;
	int zPos;
}Chunk;


void chunk_initializeChunk(int chunkY, int chunkX, int chunkZ);

void chunk_populateChunk(void);

void chunk_generateMesh(Chunk* chunk);

#endif
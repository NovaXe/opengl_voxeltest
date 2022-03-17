#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"


#define WORLD_CHUNKS_X 4
#define WORLD_CHUNKS_Y 4
#define WORLD_CHUNKS_Z 4

#define WORLD_TOTALCHUNKS (WORLD_CHUNKS_X * WORLD_CHUNKS_Y * WORLD_CHUNKS_Z)

Chunk world_chunks[WORLD_CHUNKS_X][WORLD_CHUNKS_Y][WORLD_CHUNKS_Z];

void world_generate(void);

void world_setBlock(vec3 position, BlockMutation_t* block);

bool world_checkBounds(vec3 WorldPos);

BlockMutation_t* world_getBlock(vec3 blockWorldPos);

void world_draw(void);



#endif



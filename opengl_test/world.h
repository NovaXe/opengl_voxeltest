#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"

#define WORLD_CHUNKS_Y 16
#define WORLD_CHUNKS_X 16
#define WORLD_CHUNKS_Z 16

#define WORLD_TOTALCHUNKS (WORLD_CHUNKS_Y * WORLD_CHUNKS_Z * WORLD_CHUNKS_Z)

Chunk world_chunks[WORLD_CHUNKS_Y][WORLD_CHUNKS_X][WORLD_CHUNKS_Z];

void world_generate(void);

void world_setBlock(vec3 position, BlockMutation* block);


#endif



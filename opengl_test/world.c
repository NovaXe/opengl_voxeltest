#include "chunk.h"
#include "world.h"
#include <cglm/cglm.h>


#define WORLD_HEIGHT 16
#define WORLD_WIDTH 16




void world_generate(void) 
{
	
				printf("Chunk                Y   X   Z");
	for (int chunkY = 0; chunkY < WORLD_CHUNKS_Y; chunkY++) {
		for (int chunkX = 0; chunkX < WORLD_CHUNKS_X; chunkX++) {
			for (int chunkZ = 0; chunkZ < WORLD_CHUNKS_Z; chunkZ++) {
				chunk_initializeChunk(chunkY, chunkX, chunkZ);
				printf("Initialized Chunk:   %d   %d   %d\n", chunkY, chunkX, chunkZ);


			}
		}
	}
}

void world_setBlock(vec3 pos, BlockMutation* block) 
{
	

	int chunkY = (int) pos[1] / WORLD_CHUNKS_Y;
	int chunkX = (int) pos[0] / WORLD_CHUNKS_X;
	int chunkZ = (int) pos[2] / WORLD_CHUNKS_Z;

	int blockY = (int) pos[1] % CHUNK_SIZE_Y;
	int blockX = (int) pos[0] % CHUNK_SIZE_X;
	int blockZ = (int) pos[2] % CHUNK_SIZE_Z;

	


	Chunk* chunk = &world_chunks[chunkY][chunkX][chunkZ];

	chunk->grid[blockY][blockX][blockZ] = block;


}

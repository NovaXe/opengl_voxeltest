#include "chunk.h"
#include "world.h"
#include "chunkmesh.h"
#include <cglm/cglm.h>
#include <stdbool.h>



void world_generate(void) 
{
	chunkmesh_allocate();
	printf("Chunk                X   Y   Z\n");
	for (int chunkX = 0; chunkX < WORLD_CHUNKS_X; chunkX++) {
		for (int chunkY = 0; chunkY < WORLD_CHUNKS_Y; chunkY++) {
			for (int chunkZ = 0; chunkZ < WORLD_CHUNKS_Z; chunkZ++) {
				chunk_initialize(chunkX, chunkY, chunkZ);
				printf("Initialized Chunk:   %d   %d   %d\n", chunkX, chunkY, chunkZ);


			}
		}
	}
}

void world_setBlock(vec3 blockWorldPos, BlockMutation_t* block) 
{
	

	int chunkX = (int) blockWorldPos[0] / CHUNK_SIZE_X;
	int chunkY = (int) blockWorldPos[1] / CHUNK_SIZE_Y;
	int chunkZ = (int) blockWorldPos[2] / CHUNK_SIZE_Z;

	int blockX = (int) blockWorldPos[0] % CHUNK_SIZE_X;
	int blockY = (int) blockWorldPos[1] % CHUNK_SIZE_Y;
	int blockZ = (int) blockWorldPos[2] % CHUNK_SIZE_Z;

	


	Chunk* chunk = &world_chunks[chunkX][chunkY][chunkZ];

	

	chunk->grid[blockX][blockY][blockZ] = block;
	chunk->mesh->needsRegen = true;

}

bool world_checkBounds(vec3 worldPos) {
	int x = worldPos[0];
	int y = worldPos[1];
	int z = worldPos[2];
	
	if (x >= WORLD_CHUNKS_X * CHUNK_SIZE_X || y >= WORLD_CHUNKS_Y * CHUNK_SIZE_Y || z >= WORLD_CHUNKS_Z * CHUNK_SIZE_Z)
		return false;
	else if (x < 0 || y < 0 || z < 0) {
		return false;
	}
	else {
		return true;
	}
}

BlockMutation_t* world_getBlock(vec3 blockWorldPos) {
	/*int xWorldPos = blockWorldPos[0];
	int yWorldPos = blockWorldPos[1];
	int zWorldPos = blockWorldPos[2];

	int chunkX = xWorldPos / CHUNK_SIZE_X;
	int chunkY = yWorldPos / CHUNK_SIZE_Y;
	int chunkZ = zWorldPos / CHUNK_SIZE_Z;

	int blockX = xWorldPos % CHUNK_SIZE_X;
	int blockY = yWorldPos % CHUNK_SIZE_Y;
	int blockZ = zWorldPos % CHUNK_SIZE_Z;*/

	if (!world_checkBounds(blockWorldPos)) return NULL;

	int chunkX = (int) floor(blockWorldPos[0]) / CHUNK_SIZE_X;
	int chunkY = (int) floor(blockWorldPos[1]) / CHUNK_SIZE_Y;
	int chunkZ = (int) floor(blockWorldPos[2]) / CHUNK_SIZE_Z;

	int blockX = (int) floor(blockWorldPos[0]) % CHUNK_SIZE_X;
	int blockY = (int) floor(blockWorldPos[1]) % CHUNK_SIZE_Y;
	int blockZ = (int) floor(blockWorldPos[2]) % CHUNK_SIZE_Z;


	Chunk* chunk = &world_chunks[chunkX][chunkY][chunkZ];
	BlockMutation_t* block = chunk->grid[blockX][blockY][blockZ];
	

	return block;
}


void world_draw(void) {
	//printf("world draw call\n");

	for (int x = 0; x < WORLD_CHUNKS_X; x++) {
		for (int y = 0; y < WORLD_CHUNKS_Y; y++) {
			for (int z = 0; z < WORLD_CHUNKS_Z; z++) {
				Chunk* chunk = &world_chunks[x][y][z];
				ChunkMesh* mesh = chunk->mesh;
				//printf("current chunk %d, %d, %d", x, y, z);

				if (mesh->needsRegen) {
					
					chunk_generateMesh(chunk);
					chunkmesh_update(mesh);
					//printf("mesh %d, %d, %d rebuilding\n", x, y, z);
					//chunkmesh_update(mesh);
					
				}
				chunk_checkNull(chunk);

				chunkmesh_draw(mesh, (vec3) { x * CHUNK_SIZE_X, y* CHUNK_SIZE_Y, z * CHUNK_SIZE_Z});	// gives chunk coord and 
				//printf("chunk drawn\n");


			}
		}
	}
}

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


#include "world.h"
#include "chunk.h"
#include "block.h"

void chunk_initializeChunk(int chunkY, int chunkX, int chunkZ) {
	
	for (int y = 0; y < CHUNK_SIZE_Y; y++) {
		for (int x = 0; x < CHUNK_SIZE_X; x++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				Chunk* chk = &world_chunks[chunkY][chunkX][chunkZ];
				
				if (y < 8) {
					chk->grid[y][x][z] = block_getMutation(block_stone, 0);
				}
				else {
					chk->grid[y][x][z] = block_getMutation(0, 0);
				}


			}
		}
	}


}


void chunk_populateChunk(void) {
	return;
}


void chunk_generateMesh(Chunk* chunk) {
	glGenVertexArrays(1, &chunk->mesh.VAO);
	glGenBuffers(1, &chunk->mesh.VBO);

	return;
}

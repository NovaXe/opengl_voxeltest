#include <stdbool.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "world.h"
#include "chunk.h"
#include "block.h"
#include "blockface.h"
#include "texture.h"

//int vertexCounter = 0;

void chunk_initialize(int chunkX, int chunkY, int chunkZ) {
	Chunk* chunk = &world_chunks[chunkX][chunkY][chunkZ];
	int index = chunkX * WORLD_CHUNKS_Y * WORLD_CHUNKS_X + chunkY * WORLD_CHUNKS_X + chunkZ;

	chunk->mesh = &chunkmesh_meshes[index];

	glm_vec3_copy((vec3) { chunkX * CHUNK_SIZE_X, chunkY * CHUNK_SIZE_Y, chunkZ * CHUNK_SIZE_Z}, chunk->blockPos);
	chunk_populate(chunk);

}


void chunk_populate(Chunk* chunk) {

	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (y < 8) {
					chunk->grid[x][y][z] = block_getMutation(block_stone, 0);
				}
				else {
					chunk->grid[x][y][z] = block_getMutation(0, 0);
				}
			}
		}
	}
}


void chunk_generateMesh(Chunk* chunk) {
	
	ChunkMesh* mesh = chunk->mesh;
	Vertex* vertices = &mesh->vertices;
	mesh->vertexCount = 0;

	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				
				BlockMutation_t* block = chunk->grid[x][y][z];

				chunk_addCube(chunk, mesh, block, (vec3) { x, y, z });


			}
		}
	}

	/*mesh->vertexCount = vertexCounter;
	vertexCounter = 0;*/
	mesh->needsRegen = false;
}

// TODO debug this shit
void chunk_addCube(Chunk* chunk, ChunkMesh* mesh, BlockMutation_t* block, vec3 blockPos) 
{
	if (block->blockID == 0) return;	// says fuck this shit and doesn't add it if it's air

	Vertex* vertices = &mesh->vertices;
	//fprintf(stderr, "vertex count: %d", mesh->vertexCount);		// delete this afterward retard
	bool faces[6];
	chunk_blockOcclusion(chunk, blockPos, faces);
	for (int currentFace = 0; currentFace < 6; currentFace++) {
		if (faces[currentFace]) {


			int* blockFace = &blockface_vertices[currentFace];



			// translates and adds each vertex for each triangle to the vertices array
			for (int i = 0; i < 30; i = i+5){
				Vertex vertex;
				vec3 vertexPos = { blockFace[i + 0], blockFace[i + 1], blockFace[i + 2] };

				int uOffset = blockFace[i + 3] * TEXTURE_WIDTH;
				int vOffset = blockFace[i + 4] * TEXTURE_WIDTH;

				float texU = ((block->blockID-1 % ATLAS_WIDTH) + (float) blockFace[i + 3]) / ATLAS_WIDTH;
				float texV = ((floor((block->blockID-1) / ATLAS_WIDTH)) + (float)blockFace[i + 4]) / ATLAS_WIDTH;

				




				vec2 textureCoord = { texU, texV };
				//vec2 textureCoord = { blockFace[i + 3], blockFace[i + 4] };
				// sets the vertex position
				glm_vec3_add(vertexPos, blockPos, vertex.position);
				// sets the vertex normal
				glm_vec3_copy(blockface_Normals[currentFace], vertex.normal);
				// sets the vertex texture coords
				glm_vec2_copy(textureCoord, vertex.texcoord);
				//vertex.color[0] = block->color[0];
				vertex.color[0] = (*block->color)[0];
				vertex.color[1] = (*block->color)[1];
				vertex.color[2] = (*block->color)[2];

				//glm_vec2_copy(*block->color, vertex.color);

				vertices[mesh->vertexCount] = vertex;
				mesh->vertexCount++;
				/*if (vertexCounter % 100 == 0) {
					int val = 0;
				}*/
			}
		}
	}
}


// TODO debug this shit
void chunk_blockOcclusion(Chunk* chunk, vec3 blockPos, bool* faces) 
{
	vec3 adjacentPos;
	vec3 adjacentWorldPos;
	vec3 normal;
	for (int currentFace = 0; currentFace < 6; currentFace++) {
		glm_vec3_copy(blockPos, adjacentPos);						// uses the initial blockposition

		glm_vec3_copy(blockface_Normals[currentFace], normal);	// gets the normal of the current face

		glm_vec3_add(adjacentPos, normal, adjacentPos);					// adds the normal to the face to get the adjacent block

		glm_vec3_add(adjacentPos, chunk->blockPos, adjacentWorldPos);


		if (world_checkBounds(adjacentWorldPos)) {
			BlockMutation_t* block = world_getBlock(adjacentWorldPos);
			if (block->blockID == 0) {
				faces[currentFace] = true;
			}
			else {
				faces[currentFace] = false;
			}
		}
		else {
			faces[currentFace] = true;
		}


	}
}


void chunk_checkNull(Chunk* chunk) {
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (chunk->grid[x][y][z] == NULL) {
					printf("null block");
				}
			}
		}
	}
}
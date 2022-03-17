#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "chunkmesh.h"
#include "shader.h"
#include "world.h"
#include "chunk.h"
#include "blockface.h"

ChunkMesh* chunkmesh_meshes;
Shader shader_lightingShader;


void chunkmesh_allocate() {
	chunkmesh_meshes = malloc(sizeof(ChunkMesh) * WORLD_TOTALCHUNKS);
	//chunkmesh_meshes = (ChunkMesh*) malloc(pow(16, 3) * sizeof(ChunkMesh));

	for (int i = 0; i < WORLD_TOTALCHUNKS; i++) {
		chunkmesh_init(chunkmesh_meshes + i);
	}
	shader_lightingShader = shader_compileShader("Shaders\\LightingShader.vs.glsl", "Shaders\\LightingShader.fs.glsl");
}


void chunkmesh_init(ChunkMesh* mesh) {

	mesh->vertexCount = 0;
	mesh->needsRegen = true;

	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE * sizeof(Vertex), mesh->vertices, GL_DYNAMIC_DRAW);

	// position
	glVertexAttribPointer(vertexAttribPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(vertexAttribPos);
	// normal
	glVertexAttribPointer(vertexAttribNorm, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(vertexAttribNorm);
	// texture coord
	glVertexAttribPointer(vertexAttribTex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(vertexAttribTex);

	glVertexAttribPointer(vertexAttribColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(vertexAttribColor);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void chunkmesh_update(ChunkMesh* mesh) {
	fprintf(stderr, "cmeshup: Vertex Count: %d\n", mesh->vertexCount);

	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, sizeof(Vertex) * mesh->vertexCount, mesh->vertices);
	fprintf(stderr, "cmeshup: buffer subdata updated\n");

	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertexCount, mesh->vertices, GL_STATIC_DRAW);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



}


void chunkmesh_draw(ChunkMesh* mesh, vec3 chunkPos) {
	
	

	glUseProgram(shader_lightingShader);

	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	render_wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	mat4 model;
	glm_mat4_copy(GLM_MAT4_IDENTITY, model);
	
	glm_translate(model, chunkPos);
	//glm_scale_uni(model, 1.0);

	shader_setUniformMat4(shader_lightingShader, "model", model);
	
	

	glBindVertexArray(mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
	glBindVertexArray(0);
	glUseProgram(0);


}



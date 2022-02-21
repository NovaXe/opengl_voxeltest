#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "texture.h"

unsigned int texture_textures[MAX_TEXTURES];

char* texture_textureNames[MAX_TEXTURES] = {
	"",
	"stone",
	"block2"
};

//generates texture ID and sets the textures for all supported textures
void texture_genTextures(void) 
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(256, texture_textures);
	//printf("%u", texture_textures[0]);

	for (int i = 0; i < 2; i++) {
		texture_gen(i);
	}
}


void texture_gen(unsigned int textureIndex) 
{
	glBindTexture(GL_TEXTURE_2D, texture_textures[textureIndex]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	int width, height, nrChannels;

	static char buffer[256];
	sprintf(buffer, "Textures\\Blocks\\%s.png\0", texture_textureNames[textureIndex]);
	printf("%s\n", buffer);

	static unsigned char* imgData;
	imgData = stbi_load(buffer, &width, &height, &nrChannels, 4);

	if (imgData != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
		printf("Succesfull loaded texture: %s\n", texture_textureNames[textureIndex]);
	}
	else {
		printf("Failed to load texture %d\n", textureIndex);
	}
	stbi_image_free(imgData);

	glBindTexture(GL_TEXTURE_2D, 0);
	

}
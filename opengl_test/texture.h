#ifndef TEXTURE_H
#define TEXTURE_H

#define MAX_TEXTURES 256
#define ATLAS_WIDTH 16
#define TEXTURE_WIDTH 16

typedef struct Texture_t {
	char* name;

}Texture_t;


extern unsigned int texture_textures[MAX_TEXTURES];


extern char* texture_textureNames[MAX_TEXTURES];
extern unsigned int texture_atlas;

void texture_genAtlas(void);
void texture_genTextures(void);
void texture_gen(unsigned int ID);

#endif
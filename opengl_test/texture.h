#ifndef TEXTURE_H
#define TEXTURE_H

#define MAX_TEXTURES 256


extern unsigned int texture_textures[MAX_TEXTURES];


extern char* texture_textureNames[MAX_TEXTURES];


void texture_genTextures(void);
void texture_gen(unsigned int ID);

#endif
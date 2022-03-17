#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>


#define Shader unsigned int

Shader shader_compileShader(const char* vertexPath, const char* fragmentPath);

extern Shader shader_lightingShader;


void shader_setUniformMat4(Shader shader, char* name, mat4 mat);
void shader_setUniformVec3(Shader shader, char* name, vec3 vec);
void shader_setUniformTexture(Shader shader, char* name, unsigned int textureID);



#endif

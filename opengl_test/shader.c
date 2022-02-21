#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <glad/glad.h>
#include <string.h>
#include "shader.h"


unsigned int shader_compileShader(const char* vertexPath, const char* fragmentPath)
{
	FILE* shaderFile = NULL;
	unsigned int fileSize, programID;
	char* vertexShaderCode;
	char* fragmentShaderCode;
	errno_t err;

	printf("vertexPath: %s\nfragmentPath: %s\n\n", vertexPath, fragmentPath);

	//opens and gets the size of the vertex shader file
	err = fopen_s(&shaderFile, vertexPath, "r");
	if (err != 0 || shaderFile == NULL)
	{
		fprintf(stderr, "FAILED TO OPEN %s : %s\n", vertexPath, strerror(err));
		exit(-1);
	}

	//gets the size of the file in bytes
	fseek(shaderFile, 0L, SEEK_END);
	fileSize = ftell(shaderFile);
	fseek(shaderFile, 0L, SEEK_SET);

	//reads the vertex shader file into memory
	vertexShaderCode = (char*)malloc(fileSize + 1);
	assert(vertexShaderCode != 0);
	size_t bytesRead = fread(vertexShaderCode, 1, fileSize, shaderFile);
	vertexShaderCode[bytesRead] = '\0';
	//vertexShaderCode[fileSize] = "\0";
	//closes the shaderFile and ensures the stream is NULL
	fclose(shaderFile);
	shaderFile = NULL;

	//opens and gets the size of the fragment shader file
	err = fopen_s(&shaderFile, fragmentPath, "r");
	if (err != 0 || shaderFile == NULL)
	{
		fprintf(stderr, "FAILED TO OPEN %s : %s\n", fragmentPath, strerror(err));
		exit(-1);
	}
	//gets the size of the file in bytes
	fseek(shaderFile, 0L, SEEK_END);
	fileSize = ftell(shaderFile);
	fseek(shaderFile, 0L, SEEK_SET);

	//reads the fragment shader file into memory
	fragmentShaderCode = (char*)malloc(fileSize + 1);
	assert(fragmentShaderCode != 0);
	bytesRead = fread(fragmentShaderCode, 1, fileSize, shaderFile);
	fragmentShaderCode[bytesRead] = '\0';
	//fragmentShaderCode[fileSize] = "\0";
	//closes the shaderFile and ensures the stream is NULL
	fclose(shaderFile);
	shaderFile = NULL;
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//compiles the vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}


	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}



	////compiles the fragment shader
	//fragment = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	//glCompileShader(fragment);
	//glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	//if (!success)
	//{
	//	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
	//	printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	//}

	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	//checks for linking errors
	glGetShaderiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(programID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
	}

	free(vertexShaderCode);
	free(fragmentShaderCode);

	glDeleteShader(vertex);
	glDeleteShader(fragment);


	return programID;
}
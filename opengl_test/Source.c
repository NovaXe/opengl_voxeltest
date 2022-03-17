#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <windows.h>
#include <cglm/cglm.h>


#include "shader.h"
#include "texture.h" 
#include "block.h"
#include "chunk.h"
#include "world.h"
#include "color.h"



#define SCREEN_W 1000
#define SCREEN_H 800

#define GRID_SIZE 16

#define WBlock BlockMutation_t


int g_windowWidth = SCREEN_W;
int g_windowHeight = SCREEN_H;

float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;

float g_lastMouseX = SCREEN_W / 2;
float g_lastMouseY = SCREEN_H / 2;

float g_cameraPitch = 0.0f;
float g_cameraYaw = -90.0f;

float g_cameraSpeed = 2.5f;

vec3 g_cameraPos = { 8.0f, 16.0f, 8.0f };
vec3 g_cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
vec3 g_cameraTarget;

vec3 g_lightPos = { WORLD_CHUNKS_X * CHUNK_SIZE_X, WORLD_CHUNKS_Y * CHUNK_SIZE_Y, WORLD_CHUNKS_Z * CHUNK_SIZE_Z };

vec3 g_selectionPos;

typedef enum scrollMode {
	Mode_cameraSpeed,
	Mode_lightBrightness
}scrollMode;

int g_scrollMode = Mode_cameraSpeed;



// light colors
//const vec3 color_purble		=	COLOR_PURBLE;
//const vec3 color_klorange	=	COLOR_KLORANGE;
//const vec3 color_tilder		=	COLOR_TILDER;
//const vec3 color_white		=	COLOR_WHITE;
//const vec3 color_grey		=	COLOR_GREY;
							 		
const vec3 g_lighting_color; 		



WBlock* g_currentBlock;

//WBlock* g_blockGrid[GRID_SIZE][GRID_SIZE][GRID_SIZE];


//void fillGrid();
void drawVoxels(unsigned int shader, unsigned int VAO);
void breakBlock(vec3 blockCoords);
void placeBlock(vec3 blockCoord);


void vec3_setUniform(Shader shader, char* name, vec3 vec);
void mat4_setUniform(Shader shader, char* name, mat4 mat);
void texture_setUniform(Shader shader, char* name, unsigned int textureID);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//unsigned int shader_compileShader(const char* vertexPath, const char* fragmentPath);

bool render_wireframe = false;

int main()
{
	//Sets basic context for opengl window to ensure legacy functions aren't being used
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creates a window object with dimensions and name
	GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	//sets the context of the current window what is current in the thread
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	//sets input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	//locks the mouse to the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return -1;
	}

	

	//Shader shaderProgram = shader_compileShader("Shaders\\BasicVertexShader.vs.glsl", "Shaders\\BasicFragmentShader.fs.glsl");

	Shader lightingShader = shader_compileShader("Shaders\\LightingShader.vs.glsl", "Shaders\\LightingShader.fs.glsl");
	Shader lightSourceShader = shader_compileShader("Shaders\\LightSource.vs.glsl", "Shaders\\LightSource.fs.glsl");


		////vertex data for single triangle in Normalized device coordinates
	//float vertices[] = {
	//	 0.5f,  0.5f,  0.0f,	//top right
	//	 0.5f, -0.5f,  0.0f,	//bottom right
	//	-0.5f, -0.5f,  0.0f,	//bottom left
	//	-0.5f,  0.5f,  0.0f		//top left
	//};

	//dual triangles
	/*const unsigned int obj_count = 2;
	const unsigned int obj_attrib_count = 18;*/

	/*float* vertices = (float*) malloc(obj_attrib_count * sizeof(float) * obj_count);*/

	//memcpy(vertices, );
	//TODO: figure out something for dynamic allocation of the vertices array
	//float vertices[][18] =
	//{
	//	{	//Positions				// Colors
	//		 0.50f,  0.25f,  0.0f,  1.0f,  0.0f,  0.0f,		//top
	//		 0.75f, -0.25f,  0.0f,	0.0f,  1.0f,  0.0f,		//bottom right
	//		 0.25f, -0.25f,  0.0f,	0.0f,  0.0f,  1.0f		//bottom left
	//	},
	//	{
	//		-0.50f,  0.25f,  0.0f,	0.78f,  0.10f,  1.0f,		//top				purble
	//		-0.75f, -0.25f,  0.0f,	1.0f,   0.50f,  0.0f,		//bottom left		klorange
	//		-0.25f, -0.25f,  0.0f,	0.07f,  0.7f,  0.28f		//bottom right		tilder
	//	}
	//};

	//rectangle vertices
	//float vertices[] = {
	//	// positions			// colors				  // texture coords
	//	 0.5f,  0.5f,  0.0f,	0.78f,  0.10f,  1.00f,    1.0f, 1.0f,	// top right
	//	 0.5f, -0.5f,  0.0f,	1.00f,  0.50f,  0.00f,	  1.0f, 0.0f,	// bottom right
	//	-0.5f, -0.5f,  0.0f,	0.07f,  0.80f,  0.28f,	  0.0f, 0.0f,	// bottom left
	//	-0.5f,  0.5f,  0.0f,	0.75f,  0.75f,  0.75f,	  0.0f, 1.0f	// top left
	//};


	float cube_vertices[] = {
	// vertices				// Texture Coords	// Normal Vectors
	// Back Face
	0, 0, 0,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// Bottom-left
	1, 1, 0,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
	1, 0, 0,	1.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-right       
	1, 1, 0,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
	0, 0, 0,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-left
	0, 1, 0,	0.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-left
	// Front face					   			  
	0, 0, 1,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
	1, 0, 1,	1.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-right
	1, 1, 1,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
	1, 1, 1,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
	0, 1, 1,	0.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-left
	0, 0, 1,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
	// Left face								  
	0, 1, 1,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
	0, 1, 0,	1.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// top-left
	0, 0, 0,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
	0, 0, 0,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
	0, 0, 1,	0.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-right
	0, 1, 1,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
	// Right face	  		 	   			   	  
	1, 1, 1,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
	1, 0, 0,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
	1, 1, 0,	1.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// top-right         
	1, 0, 0,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
	1, 1, 1,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
	1, 0, 1,	0.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// bottom-left     
	// Bottom face	  		 	   			   	  
	0, 0, 0,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
	1, 0, 0,	1.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-left
	1, 0, 1,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
	1, 0, 1,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
	0, 0, 1,	0.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-right
	0, 0, 0,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
	// Top face				  		   		  
	0, 1, 0,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
	1, 1, 1,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
	1, 1, 0,	1.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-right     
	1, 1, 1,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
	0, 1, 0,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
	0, 1, 1,	0.0f, 0.0f,		0.0f,  1.0f,  0.0f 		// bottom-left        
	};

	//float cube_vertices[] = {
	//	// vertices				// Texture Coords	// Normal Vectors
	//	// Back Face
	//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// Bottom-left
	//	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
	//	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-right       
	//	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
	//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-left
	//	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-left
	//	// Front face					   			  
	//	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
	//	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-right
	//	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
	//	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
	//	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-left
	//	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
	//	// Left face								  
	//	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
	//	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// top-left
	//	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
	//	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
	//	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-right
	//	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
	//	// Right face	  		 	   			   	  
	//	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
	//	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
	//	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// top-right         
	//	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
	//	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
	//	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// bottom-left     
	//	// Bottom face	  		 	   			   	  
	//	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
	//	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-left
	//	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
	//	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
	//	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-right
	//	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
	//	// Top face				  		   		  
	//	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
	//	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
	//	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-right     
	//	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
	//	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
	//	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,		0.0f,  1.0f,  0.0f 		// bottom-left        
	//};



	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		// Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		// top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		// bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		// top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		// bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		// top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		// bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		// bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		// top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		// top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		// top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		// bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		// top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		// bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		// top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		// bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,		// top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		// bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		// bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		// bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		// top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		// top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		// top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		// bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		// top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f		// bottom-left        
	};


	unsigned int indices[] =
	{
		0
	};



	float selectionVertices[] =
	{
		//back
		0, 0, 0,
		1, 0, 0,
		0, 1, 0,
		1, 1, 0,
		//front
		0, 0, 1,
		1, 0, 1,
		0, 1, 1,
		1, 1, 1,

		 //left
		0, 0, 0, 
		0, 1, 0, 
		0, 0, 1, 
		0, 1, 1, 
		 //right
		1, 0, 0,  
		1, 1, 0,  
		1, 0, 1,  
		1, 1, 1,  

		 //bottom
		0, 0, 0,
		1, 0, 0,
		0, 0, 1,
		1, 0, 1,
		 //top
		0, 1, 0,
		1, 1, 0,
		0, 1, 1,
		1, 1, 1,



	};







	/*unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
	};*/

	//float vertices[] = {
	//	//Coords				//Colors
	//	 0.5f,  -0.5f,   0.0f,	0.78f,  0.10f,  1.00f,		//top				purble
	//	-0.5f,  -0.5f,   0.0f,	1.00f,  0.50f,  0.00f,		//bottom left		klorange
	//	 0.0f,   0.5f,   0.0f,	0.07f,  0.80f,  0.28f		//bottom right		tilder
	//};
	//float texCoords[] = {
	//	0.0f, 0.0f,		//bottom left
	//	1.0f, 0.0f,		//bottom right
	//	0.5f, 1.0f		//top middle
	//};





	//Generates neccessary drawing/rendering objects
	//vertex buffer, vertex array, element buffer
	//TODO: dynamically allocate objects
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//vertex texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//safe to unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//The VAO can also be unbound to prevent other VAO calls from modifying the VAO, doesn't really happen though
	//In order to modify another VAO you need to call glBIndVertexArray so there isn't much reason to unbind the VAO
	glBindVertexArray(0);

	//also safe to unbind the element buffer object as it is also stored in the vertex array
	//must unbind after unbinding the vertex array otherwise the unbind call is stored as well
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// VBO already contains vertex data and doesn't need to be generated again
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//sets the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	//texture_genTextures();

	unsigned int quad_VBO;
	unsigned int selectionVAO;
	glGenBuffers(1, &quad_VBO);
	glGenVertexArrays(1, &selectionVAO);

	glBindVertexArray(selectionVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(selectionVertices), selectionVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	/*
	unsigned int texture_list[2];
	glGenTextures(2, texture_list);
	glBindTexture(GL_TEXTURE_2D, texture_list[0]);
	// sets the textures' wrapping and filtering options for the currently bound object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//loads and generates the tectures making sure it actually is loaded
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load("Textures\\showerman.png", &width, &height, &nrChannels, 4);
	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(imgData);


	glBindTexture(GL_TEXTURE_2D, texture_list[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	imgData = stbi_load("Textures\\rodent.png", &width, &height, &nrChannels, 0);
	printf("failed b/c: %s\n", stbi_failure_reason());
	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Failed to load texture2\n");
	}
	stbi_image_free(imgData);
	*/

	

	//texture_genTextures();
	texture_genAtlas();

	block_defineBlocks();

	world_generate();

	//binds the shaderProgram
	//glUseProgram(lightingShader);
	//glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	//glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
	


	unsigned int currentShader;
	
	unsigned int transformLoc;
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int projectionLoc;

	unsigned int objectColorLoc;
	unsigned int lightColorLoc;
	unsigned int lightPosLoc;
	unsigned int viewPosLoc;

	//Initial conditions for the render Loop
	unsigned int frames, total_frames;
	frames = 0;
	total_frames = 0;
	float lastSec = 0.0;

	bool hell = true;

	//sets the color of the light
	
	g_currentBlock = block_getMutation(block_stone, 0);
	
	glm_vec3_copy(color_white, g_lighting_color);
	
	
	//fills the voxel grid with generic blocks			////function now deprecated and moved into the world.c file
	//fillGrid();



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//IN CASE IM FUCKING BLIND \/ \/ \/ \/ \/ \/ THIS IS THE RENDER LOOP
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		frames++;
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		render_wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		float timeValue = g_lastFrame;
		
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_list[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_list[1]);*/

		shader_lightingShader;
		currentShader = shader_lightingShader;

		glUseProgram(shader_lightingShader);
		
		//currentShader = lightingShader;

		transformLoc = glGetUniformLocation(currentShader, "transform");
		modelLoc = glGetUniformLocation(currentShader, "model");
		viewLoc = glGetUniformLocation(currentShader, "view");
		projectionLoc = glGetUniformLocation(currentShader, "projection");

		objectColorLoc = glGetUniformLocation(currentShader, "objectColor");
		lightColorLoc = glGetUniformLocation(currentShader, "lightColor");
		lightPosLoc = glGetUniformLocation(currentShader, "lightPos");
		viewPosLoc = glGetUniformLocation(currentShader, "viewPos");


		


		//sets the camera direction
		glm_vec3_add(g_cameraPos, g_cameraFront, g_cameraTarget);

		mat4 view;
		glm_lookat(g_cameraPos, g_cameraTarget, g_cameraUp, view);

		mat4 projection;
		glm_perspective(glm_rad(90.0f), ((float)g_windowWidth / (float)g_windowHeight), 0.1f, 1000.0f, projection);

		//glm_vec3_copy((vec3) { 1.0 + sin(timeValue) * 2.0, 2.0f, 2.0f }, g_lightPos);

		shader_setUniformMat4(currentShader, "view", view);
		shader_setUniformMat4(currentShader, "projection", projection);

		shader_setUniformVec3(shader_lightingShader, "lightColor", g_lighting_color);
		shader_setUniformVec3(shader_lightingShader, "lightPos", g_lightPos);
		shader_setUniformVec3(shader_lightingShader, "viewPos", g_cameraPos);

		// shader_setUniformVec3(shader_lightingShader, "objectColor", color_white);

		//draws the voxel grid
		//drawVoxels(currentShader, VAO);
		world_draw();


		vec3 blockSel;
		vec3 dir;

		glm_vec3_sub(g_cameraTarget, g_cameraPos, dir);
		glm_vec3_add(g_cameraTarget, dir, blockSel);
		glm_vec3_copy(blockSel, g_selectionPos);		//copies into global variable

		//glm_vec3_print(blockSel, stderr);
		//glm_vec3_print(g_selectionPos, stderr);


		mat4 model = GLM_MAT4_IDENTITY_INIT;
		//glm_translate(model, (vec3) { floor(g_selectionPos[0] + 0.5), floor(g_selectionPos[1] + 0.5), floor(g_selectionPos[2] + 0.5) });

		glm_translate(model, (vec3) { floor(g_selectionPos[0]), floor(g_selectionPos[1]), floor(g_selectionPos[2]) });
		//glm_translate(model, (vec3) { 0.5, 0.5, 0.5 });
		//glm_translate(model, (vec3) { floor(g_selectionPos[0]), floor(g_selectionPos[1]), floor(g_selectionPos[2]) });


		glUseProgram(currentShader);

		shader_setUniformMat4(currentShader, "model", model);
		shader_setUniformVec3(currentShader, "objectColor", GLM_VEC3_ZERO);

		//glDisable(GL_CULL_FACE);

		if (1) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(4);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glLineWidth(1);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		

		//glEnable(GL_CULL_FACE);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//glUniform3fv(objectColorLoc, 1, color_klorange);


		//double relX = cos(timeValue * 4) / 1.75;
		//double relY = sin(timeValue * 4) / 1.75;

		//glm_mat3_copy((vec3) { relX, relY, 0.0 }, translation);



		//glm_mat4_copy(GLM_MAT4_IDENTITY, model);

		//glm_translate(model, (vec3) { relX, relY, 0.0 });
		//glm_rotate(model, timeValue * 4 - glm_rad(90), GLM_ZUP);
		//glm_scale(model, (vec3) { 0.25, 0.25, 0.25 });
		//
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
		
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/*glDrawArrays(GL_TRIANGLES, 0, 36);*/
		
		//unbind the regular vertex array
		/*glBindVertexArray(0);*/


		//switches to the light source shader for drawing the lamp
		

		glUseProgram(lightSourceShader);
		
		currentShader = lightSourceShader;
		/*modelLoc = glGetUniformLocation(currentShader, "model");
		viewLoc = glGetUniformLocation(currentShader, "view");
		projectionLoc = glGetUniformLocation(currentShader, "projection");*/

		//lightColorLoc = glGetUniformLocation(currentShader, "lightColor");
		

		vec3 translation = GLM_VEC3_ZERO_INIT;
		glm_mat4_copy(GLM_MAT4_IDENTITY, model);
		glm_translate(model, translation);
		//glm_rotate(model, glm_rad(0.0f), GLM_XUP);
		glm_scale(model, (vec3) { 0.25, 0.25, 0.25 });


		glm_mat4_copy(GLM_MAT4_IDENTITY, model);
		glm_translate(model, g_lightPos);
		glm_scale_uni(model, 0.5f);

		/*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);*/

		shader_setUniformMat4(currentShader, "model", model);
		shader_setUniformMat4(currentShader, "view", view);
		shader_setUniformMat4(currentShader, "projection", projection);

		shader_setUniformVec3(currentShader, "lightColor", g_lighting_color);

		//glUniform3fv(lightColorLoc, 1, lighting_color);
		

		// binds the light source vertex array and draws the lamp object
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		



		// NOTE : binding and ubinding a single VAO is unneccesary unless there is multiple, only doing this for organization
		
		////swap buffers and poll IO events (key presses and mouse input)
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		float currentFrame = glfwGetTime();
		g_deltaTime = currentFrame - g_lastFrame;
		g_lastFrame = currentFrame;

		block_defineMutation(block_stone, 4, (vec3) { sin(currentFrame + 4), sin(currentFrame), sin(currentFrame + 8) });

		//end_t = clock();
		//total_t = (double) (end_t - initial_t) / CLOCKS_PER_SEC;
		//printf("total time: %d\n", total_t);
		float deltaSec = currentFrame - lastSec;


		if (deltaSec >= 1.0)
		{
			//printf("FPS: %d\n", frames);
			char f[16];

			sprintf(f, "%d", frames);
			glfwSetWindowTitle(window, f);
			total_frames += frames;
			frames = 0;
			lastSec = currentFrame;
			//printf("x: %f, y: %f\n", xpos, g_windowHeight - ypos);

			

		}

		if (!hell) {
			MessageBox(0, L"your life has been cancelled", L"important message", MB_OK);
			hell = true;
		}

	}
	
	/*int average_framerate = (int)total_frames / ((clock() - initial_t) / CLOCKS_PER_SEC);
	printf("------------------\nAverages FPS: %d\n", average_framerate);*/


	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	//frees all allocated resources
	glfwTerminate();
	return 0;
}

//	goes into something like render.c to handle any forms of rendering
//	TODO: Split mesh generation from the drawing of the voxels so the checks aren't performed every frame
void drawVoxels(unsigned int shader, unsigned int VAO) 
{
	mat4 model;

	//vec3_setUniform(shader, "objectColor", color_grey);
	shader_setUniformVec3(shader, "lightColor", g_lighting_color);
	shader_setUniformVec3(shader, "lightPos", g_lightPos);
	shader_setUniformVec3(shader, "viewPos", g_cameraPos);


	//vec3_setUniform(shader, "objectColor", color_grey);

	

	glBindVertexArray(VAO);

	unsigned int ID;

	
	for(int chunkX=0; chunkX<1; chunkX++)
	{
	

	for (int j = 0; j < GRID_SIZE; j++) {
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int k = 0; k < GRID_SIZE; k++) {
				glm_mat4_copy(GLM_MAT4_IDENTITY, model);
				glm_translate(model, (vec3) { (float) chunkX * CHUNK_SIZE_X, (float)0, (float)0 });
				glm_translate(model, (vec3) { (float) j, (float) i, (float) k });
				glm_scale(model, (vec3) { 1.00, 1.00, 1.00 });

				//(vec3) {0.25, 0.25, 0.25}
				//{1.00, 1.00, 1.00}
				shader_setUniformMat4(shader, "model", model);

				Chunk* currentChunk = &world_chunks[chunkX][0][0];

				WBlock* current = currentChunk->grid[j][i][k];

				Block_t* currentBlock = block_get(current->blockID);
				unsigned int currentTexture = block_get(current->blockID)->textureID;

				shader_setUniformVec3(shader, "objectColor", *(current->color));
				shader_setUniformTexture(shader, "objectTexture", current->blockID);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, currentTexture);


				//checks -z and +z faces
				if (current->blockID == 0) {
					continue;
				}


				if (k - 1 >= 0) {
					ID = currentChunk->grid[j][i][k - 1]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}

				if (k + 1 < GRID_SIZE) {
					ID = currentChunk->grid[j][i][k + 1]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 6, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 6, 6);
				}


				//checks -x and +x faces
				if (j - 1 >= 0) {
					ID = currentChunk->grid[j - 1][i][k]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 12, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 12, 6);
				}

				if (j + 1 < GRID_SIZE) {
					ID = currentChunk->grid[j + 1][i][k]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 18, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 18, 6);
				}

				//checks -y and +y faces

				if (i - 1 >= 0){
					ID = currentChunk->grid[j][i-1][k]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 24, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 24, 6);
				}

				if (i + 1 < GRID_SIZE) {
					ID = currentChunk->grid[j][i+1][k]->blockID;
					if (ID == 0) {
						glDrawArrays(GL_TRIANGLES, 30, 6);
					}
				}
				else {
					glDrawArrays(GL_TRIANGLES, 30, 6);
				}


				
				

				//glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

	}
	

	glBindVertexArray(0);
}
// NOT USED ^^^^^^^^^^^^^^^^

////---------------------------------------------------------------------------------
////	wrap into into something like world.c and consolidate to one setting function
void breakBlock(vec3 blockCoord) 
{
	int x = blockCoord[0];
	int y = blockCoord[1];
	int z = blockCoord[2];

	WBlock* air = block_getMutation(block_air, 0);

	world_setBlock(blockCoord, air);

	/*if ((x >= 0 && x < GRID_SIZE) && (y >= 0 && y < GRID_SIZE) && (z >= 0 && z < GRID_SIZE)) {
		g_blockGrid[y][x][z] = air;
	}*/
	
}

void placeBlock(vec3 blockCoord) 
{
	int x = blockCoord[0];
	int y = blockCoord[1];
	int z = blockCoord[2];

	WBlock* block = g_currentBlock;
	if (world_checkBounds(blockCoord)) {
		world_setBlock(blockCoord, block);
	}
}

//void fillGrid() 
//{
//	WBlock* stone = block_defineMutation(block_stone, 0, color_white);
//	WBlock* air = block_defineMutation(block_air, 0, color_white);
//	
//
//
//	g_currentBlock = block_getMutation(block_stone, 0);
//
//	for (int j = 0; j<GRID_SIZE; j++) {
//		for (int i = 0; i < GRID_SIZE; i++) {
//			for (int k = 0; k < GRID_SIZE; k++) {
//				if(j < 10)
//					g_blockGrid[j][i][k] = stone;
//				else
//					g_blockGrid[j][i][k] = air;
//				/*printf("block ID: %d", c.ID);*/
//			}
//		}
//	}
//}
////---------------------------------------------------------------------------------



////---------------------------------------------------------------------------------
////	Move to the shader source file
void vec3_setUniform(Shader shader, char* name, vec3 vec) 
{
	glUniform3fv(glGetUniformLocation(shader, name), 1, vec);
}

void texture_setUniform(Shader shader, char* name, unsigned int textureID) 
{
	glUniform1i(glGetUniformLocation(shader, name), 0);
}

void mat4_setUniform(Shader shader, char* name, mat4 mat) 
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, mat);
}
////---------------------------------------------------------------------------------



////	Maybe I should move all of the opengl boiler plate shit and inputs into seperate screen and input handling source files
//function for resetting the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	float deltaX = xpos - g_lastMouseX;
	float deltaY = -1 * (ypos - g_lastMouseY);
	g_lastMouseX = xpos;
	g_lastMouseY = ypos;
	
	const float sensitivity = 0.05f;
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	g_cameraYaw += deltaX;
	g_cameraPitch += deltaY;

	if (g_cameraPitch > 89.0f)
		g_cameraPitch = 89.0f;
	if (g_cameraPitch < -89.0f)
		g_cameraPitch = -89.0f;

	



	vec3 direction = GLM_VEC3_ZERO_INIT;
	direction[0] = cos(glm_rad(g_cameraYaw)) * cos(glm_rad(g_cameraPitch));
	direction[1] = sin(glm_rad(g_cameraPitch));
	direction[2] = sin(glm_rad(g_cameraYaw)) * cos(glm_rad(g_cameraPitch));

	glm_vec3_normalize_to(direction, g_cameraFront);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		breakBlock((vec3) { floor(g_selectionPos[0]), floor(g_selectionPos[1]), floor(g_selectionPos[2]) });
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		placeBlock((vec3) { floor(g_selectionPos[0]), floor(g_selectionPos[1]), floor(g_selectionPos[2]) });
	}

	
	
}

void mouse_scroll_callback(GLFWwindow* widnow, double xoffset, double yoffset) 
{

	if (g_scrollMode == Mode_cameraSpeed) {

		if (g_cameraSpeed + yoffset < 0)
			g_cameraSpeed = 0;
		else
			g_cameraSpeed += (float)yoffset;

	}
	
	if (g_scrollMode == Mode_lightBrightness) {
		
		float maxBrightness = 10.0;

		if (g_lighting_color[0] == maxBrightness || g_lighting_color[1] == maxBrightness || g_lighting_color[2] == maxBrightness){
			if (yoffset < 0) {
				glm_vec3_adds(g_lighting_color, yoffset * 0.01, g_lighting_color);
			}
		}
		else {
			glm_vec3_adds(g_lighting_color, yoffset * 0.01, g_lighting_color);
		}

			
		//glmc_vec3_norm()

		//glmc_vec3_copy()
	}
	
}



//processes inputs
void processInput(GLFWwindow* window)
{
	vec3 dir;
	float cameraSpeed = g_cameraSpeed * g_deltaTime;
	//Uses the Esc key to close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)		//forward
	{
		//glm_vec3_dup(g_cameraFront, dir);
		glm_vec3_cross(g_cameraUp, g_cameraFront, dir);
		glm_vec3_cross(GLM_YUP, dir, dir);
		glm_vec3_negate(dir);
		glm_vec3_normalize(dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)		//backwward
	{
		//glm_vec3_dup(g_cameraFront, dir);
		glm_vec3_cross(g_cameraUp, g_cameraFront, dir);
		glm_vec3_cross(GLM_YUP, dir, dir);
		glm_vec3_normalize(dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)		//left
	{
		glm_vec3_cross(g_cameraUp, g_cameraFront, dir);
		glm_vec3_normalize(dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)		//right
	{
		glm_vec3_cross(g_cameraUp, g_cameraFront, dir);
		glm_vec3_normalize(dir);
		glm_vec3_negate(dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)		//right
	{
		glm_vec3_dup(g_cameraUp, dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)		//right
	{
		glm_vec3_dup(g_cameraUp, dir);
		glm_vec3_negate(dir);
		glm_vec3_muladds(dir, cameraSpeed, g_cameraPos);
	}

	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	static bool H_state = false;
	//printf("KEYPRESS EVENT\nKey: %c || Action: %d\n--------------------\n", (char)key, action);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		render_wireframe = !render_wireframe;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		g_cameraSpeed = 2.5f;
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		H_state = !H_state;
		if (H_state)
			g_scrollMode = Mode_lightBrightness;
		else
			g_scrollMode = Mode_cameraSpeed;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		if (world_checkBounds(g_selectionPos)) {
			BlockMutation_t* block = world_getBlock(g_selectionPos);
			char* name = block_definitions[block->blockID].name;
			
			printf("Block: %s at position: %d, %d, %d\n", name, (int) floor(g_selectionPos[0]), (int) floor(g_selectionPos[1]), (int) floor(g_selectionPos[2]));
		}
		else {
			printf("Selection out of bounds\n");
		}
		
		
		

	}


	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_1:
				g_currentBlock = block_getMutation(block_stone, 0);
				break;
			case GLFW_KEY_2:
				g_currentBlock = block_getMutation(block_stone, 1);
				break;
			case GLFW_KEY_3:
				g_currentBlock = block_getMutation(block_stone, 2);
				break;
			case GLFW_KEY_4:
				g_currentBlock = block_getMutation(block_stone, 3);
				break;
			case GLFW_KEY_5:
				g_currentBlock = block_getMutation(block_stone, 4);
				break;
			case GLFW_KEY_6:
				g_currentBlock = block_getMutation(block_dirt, 0);
				break;
			case GLFW_KEY_7:
				g_currentBlock = block_getMutation(block_dirt, 1);
				break;
			case GLFW_KEY_8:
				g_currentBlock = block_getMutation(block_dirt, 2);
				break;
			case GLFW_KEY_9:
				g_currentBlock = block_getMutation(block_dirt, 3);
				break;


		}
	}
}

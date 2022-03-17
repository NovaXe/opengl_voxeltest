#include "blockface.h"
#include <cglm/cglm.h>


vec3 blockface_Normals[] = {
	{ 0.0,  1.0,  0.0 },	// +y
	{ 0.0, -1.0,  0.0 },	// -y

	{ 1.0,  0.0,  0.0 },	// +x
	{-1.0,  0.0,  0.0 },	// -x

	{ 0.0,  0.0,  1.0 },	// +z
	{ 0.0,  0.0, -1.0 }		// -z
};



int blockface_vertices[6][30] = {


	// +y
	{
	0,1,0,  0,1,		// top-left
	1,1,1,  1,0,		// bottom-right
	1,1,0,  1,1,		// top-right

	1,1,1,  1,0,		// bottom-right
	0,1,0,  0,1,		// top-left
	0,1,1,  0,0,		// bottom-left        
	},

	// -y
	{
	0,0,0,  0,1,		// top-left
	1,0,0,  0,1,		// top-left
	1,0,1,  0,0,		// bottom-left
	  
	1,0,1,  0,0,		// bottom-left
	0,0,1,  1,0,		// bottom-right
	0,0,0,  1,1,		// top-right 
	},

	// +x
	{
	1,1,1,  0,1,		// top-left
	1,0,0,  1,0,		// bottom-right
	1,1,0,  1,1,		// top-right

	1,0,0,  1,0,		// bottom-right
	1,1,1,  0,1,		// top-left
	1,0,1,  0,0,		// bottom-left   
	},
	// -x
	{
	0,1,1,  1,1,		// top-right
	0,1,0,  0,1,		// top-left
	0,0,0,  0,0,		// bottom-left

	0,0,0,  0,0,		// bottom-left
	0,0,1,  1,0,		// bottom-right
	0,1,1,  1,1,		// top-right
	},

	// +z
	{
	0,0,1,  0,0,		// bottom-left
	1,0,1,  1,0,		// bottom-right
	1,1,1,  1,1,		// top-right

	1,1,1,  1,1,		// top-right
	0,1,1,  0,1,		// top-left
	0,0,1,  0,0			// bottom-left
	},
	// -z
	{
	0,0,0,  1,0,		// Bottom-left
	1,1,0,  0,1,		// top-right
	1,0,0,  0,0,		// bottom-right         

	1,1,0,  0,1,		// top-right
	0,0,0,  1,0,		// bottom-left
	0,1,0,  1,1,		// top-left
	}

};

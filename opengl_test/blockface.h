#ifndef BLOCKFACE_H
#define BLOCKFACE_H

#include <cglm/cglm.h>

typedef enum blockface_faces {
	blockTop,		// +y
	blockBottom,	// -y
	blockEast,		// +x
	blockWest,		// -x
	blockNorth,		// +z
	blockSouth		// -z
}blockface_faces;

extern vec3 blockface_Normals[];

extern int blockface_vertices[6][30];

#endif
#ifndef BLOCK_H
#define BLOCK_H
#define MAX_BLOCKS 256

#include <cglm/cglm.h>
#include "color.h"


typedef enum block_NameID {
	block_air = 0,
	block_stone = 1,
	block_dirt = 2
}block_NameID;




typedef struct Block_t {
	unsigned int textureID;
	char* name;
}Block_t;


typedef struct BlockMutation_t {
	unsigned int blockID;
	vec3* color;
}BlockMutation_t;



extern Block_t block_definitions[MAX_BLOCKS];
extern BlockMutation_t block_mutations[MAX_BLOCKS * 8];




void block_defineBlocks(void);

Block_t* block_define(unsigned __int8 ID, char* name);

BlockMutation_t* block_defineMutation(unsigned __int8 blockID, unsigned __int8 mutationID, vec3 color);


Block_t* block_get(unsigned __int8 blockID);

BlockMutation_t* block_getMutation(unsigned __int8 blockID, unsigned __int8 mutationID);

#endif

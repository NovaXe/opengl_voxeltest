#ifndef BLOCK_H
#define BLOCK_H
#define MAX_BLOCKS 256

#include <cglm/cglm.h>
#include "color.h"


typedef enum block_NameID {
	block_air,
	block_stone
}block_NameID;




typedef struct Block {
	unsigned int textureID;
	char* name;
}Block;


typedef struct BlockMutation {
	unsigned int blockID;
	vec3* color;
}BlockMutation;



extern Block block_definitions[MAX_BLOCKS];
extern BlockMutation block_mutations[MAX_BLOCKS * 8];




void block_defineBlocks(void);

Block* block_define(unsigned __int8 ID, char* name, unsigned int textureID);

BlockMutation* block_defineMutation(unsigned __int8 blockID, unsigned __int8 mutationID, vec3 color);


Block* block_get(unsigned __int8 blockID);

BlockMutation* block_getMutation(unsigned __int8 blockID, unsigned __int8 mutationID);

#endif

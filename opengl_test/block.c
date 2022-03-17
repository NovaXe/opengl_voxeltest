#include "block.h"
#include "texture.h"
#include "color.h"

Block_t block_definitions[MAX_BLOCKS];
BlockMutation_t block_mutations[MAX_BLOCKS * 8];
unsigned int texture_textures[MAX_TEXTURES];

const vec3 block_defaultColor = COLOR_WHITE;

void block_defineBlocks(void) {
	
	block_define(block_air, "air");

	block_define(block_stone, "stone");
	block_defineMutation(block_stone, 1, color_klorange);
	block_defineMutation(block_stone, 2, color_purble);
	block_defineMutation(block_stone, 3, color_tilder);

	block_define(block_dirt, "dirt");
	block_defineMutation(block_dirt, 1, color_klorange);
	block_defineMutation(block_dirt, 2, color_purble);
	block_defineMutation(block_dirt, 3, color_tilder);
}





Block_t* block_define(unsigned __int8 ID, char* name) 
{
	Block_t* block = &block_definitions[ID];
	BlockMutation_t* blockMutation = &block_mutations[ID * 8];

	block->name = name;
	//block->textureID = textureID;

	blockMutation->blockID = ID;
	blockMutation->color = block_defaultColor;

	return block;
}

BlockMutation_t* block_defineMutation(unsigned __int8 blockID, unsigned __int8 mutationID, vec3 color) 
{
	BlockMutation_t* blockM = &block_mutations[blockID * 8 + mutationID];

	blockM->blockID = blockID;
	blockM->color = color;

	

	return blockM;
}


Block_t* block_get(unsigned __int8 blockID) 
{
	return &block_definitions[blockID];
}

BlockMutation_t* block_getMutation(unsigned __int8 blockID, unsigned __int8 mutationID) 
{
	return &block_mutations[blockID * 8 + mutationID];
}

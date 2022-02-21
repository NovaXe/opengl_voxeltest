#include "block.h"
#include "texture.h"
#include "color.h"

Block block_definitions[MAX_BLOCKS];
BlockMutation block_mutations[MAX_BLOCKS * 8];
unsigned int texture_textures[MAX_TEXTURES];

const vec3 block_defaultColor = COLOR_WHITE;

void block_defineBlocks(void) {
	
	block_define(block_air, "air", 0);
	block_define(block_stone, "stone", texture_textures[block_stone]);
	block_defineMutation(block_stone, 1, color_klorange);
	block_defineMutation(block_stone, 2, color_purble);
	block_defineMutation(block_stone, 3, color_tilder);
}





Block* block_define(unsigned __int8 ID, char* name, unsigned int textureID) 
{
	Block* block = &block_definitions[ID];
	BlockMutation* blockMutation = &block_mutations[ID * 8];

	block->name = name;
	block->textureID = textureID;

	blockMutation->blockID = ID;
	blockMutation->color = block_defaultColor;

	return block;
}

BlockMutation* block_defineMutation(unsigned __int8 blockID, unsigned __int8 mutationID, vec3 color) 
{
	BlockMutation* blockM = &block_mutations[blockID * 8 + mutationID];

	blockM->blockID = blockID;
	blockM->color = color;

	

	return blockM;
}


Block* block_get(unsigned __int8 blockID) 
{
	return &block_definitions[blockID];
}

BlockMutation* block_getMutation(unsigned __int8 blockID, unsigned __int8 mutationID) 
{
	return &block_mutations[blockID * 8 + mutationID];
}

#include "Blocks.h"

namespace Blocks {
	std::map<int, Block*> blockList;

	Block* Air;
	Block* Dirt;
	Block* Stone;
	Block* Grass;
	Block* GlowstoneLamp;
	Block* Water;
}
void Blocks::initializeBlocks(GraphicsManager* gm) {
	Air = registerBlock(0, new BlockAir(), gm);
	Dirt = registerBlock(1, new BlockDirt(), gm);
	Stone = registerBlock(2, new BlockStone(), gm);
	Grass = registerBlock(3, new BlockGrass(), gm);
	GlowstoneLamp = registerBlock(4, new BlockGlowstoneLamp(), gm);
	Water = registerBlock(5, new BlockWater(), gm);
}

Block* Blocks::registerBlock(int id, Block * block, GraphicsManager* gm) {
	blockList.insert({id, block});
	loadBlockData(block, gm);
	return block;
}

void Blocks::loadBlockData(Block* block, GraphicsManager* gm) {
	for (int i = 0; i < 6; ++i) {
		if (block->textures[i].path != "")
			block->textures[i].texId = gm->getArrayTexture(block->textures[i].path);
	}
}

Block* Blocks::getBlock(int blockId) {
	auto val = blockList.find(blockId);
	if (val != blockList.end())
		return blockList[blockId];
	return Air;
}
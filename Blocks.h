#pragma once

#include <map>

#include "GraphicsManager.h"

#include "Block.h"
#include "BlockAir.h"
#include "BlockDirt.h"
#include "BlockStone.h"
#include "BlockGrass.h"
#include "BlockGlowstoneLamp.h"
#include "BlockWater.h"



namespace Blocks {
	extern Block* Air;
	extern Block* Dirt;
	extern Block* Stone;
	extern Block* Grass;
	extern Block* GlowstoneLamp;
	extern Block* Water;
	
	void initializeBlocks(GraphicsManager* gm);
	Block * registerBlock(int id, Block * block, GraphicsManager* gm);
	void loadBlockData(Block * block, GraphicsManager* gm);

	Block* getBlock(int blockId);
};


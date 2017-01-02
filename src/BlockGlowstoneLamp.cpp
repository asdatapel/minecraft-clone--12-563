#include "BlockGlowstoneLamp.h"



BlockGlowstoneLamp::BlockGlowstoneLamp() {
	lightOutput[0] = 15;
	lightOutput[1] = 0;
	lightOutput[2] = 0;
	textures[0].path = "textures/redstone_lamp_on.png";
	textures[1].path = "textures/redstone_lamp_on.png";
	textures[2].path = "textures/redstone_lamp_on.png";
	textures[3].path = "textures/redstone_lamp_on.png";
	textures[4].path = "textures/redstone_lamp_on.png";
	textures[5].path = "textures/redstone_lamp_on.png";
}


BlockGlowstoneLamp::~BlockGlowstoneLamp() {}

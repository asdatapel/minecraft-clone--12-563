#pragma once
#include <string>
#include "Types.h"
class Block
{
public:
	Block();
	~Block();
	
	int id;
	LightLevel lightOutput[3];

	bool isTransparent;

	struct {
		std::string path;
		int texId;
	} textures[6];
};


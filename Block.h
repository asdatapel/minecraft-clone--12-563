#pragma once
#include <string>
class Block
{
public:
	Block();
	~Block();
	
	int id;
	int lightOutput;

	bool isTransparent;

	struct {
		std::string path;
		int texId;
	} textures[6];
};


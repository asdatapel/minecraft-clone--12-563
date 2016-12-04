#pragma once
#include "vec.h"

class AABB
{
public:
	AABB();
	~AABB();

	void setPosition(fvec3 newPos);
	fvec3 getPosition();
	void setSize(fvec3 newSize);
	fvec3 getSize();

	fvec3 position;
	fvec3 size;
};


#include "AABB.h"



AABB::AABB()
{
}


AABB::~AABB()
{
}

void AABB::setPosition(fvec3 newPos)
{
	position = newPos;
}

fvec3 AABB::getPosition()
{
	return position;
}

void AABB::setSize(fvec3 newSize)
{
	size = newSize;
}

fvec3 AABB::getSize()
{
	return size;
}

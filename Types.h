#pragma once

#include "vec.h"
#include "BoundingBox.h"
#include "AABB.h"

struct RayCollision {
	bool isCollision;
	ivec2 chunk;
	fvec3 blockPos;
	float distance;
	int side;
};

struct lightPropogationNode {
	fvec3 position;
	int lightLevel;
};

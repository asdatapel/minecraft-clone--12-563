#pragma once
#include "Types.h"

namespace Const {
	const int WINDOW_WIDTH = 1920;
	const int WINDOW_HEIGHT = 1080;

	const int CHUNK_SIZE_X = 16;
	const int CHUNK_SIZE_Z = 16;
	const int CHUNK_SIZE_Y = 256;

	const fvec3 CHUNK_SIZE = fvec3(CHUNK_SIZE_X, CHUNK_SIZE_Z, CHUNK_SIZE_Y);

	const int WORLD_CHUNKS_X = 1;
	const int WORLD_CHUNKS_Z = 1;

	const int WORLD_SIZE_X = CHUNK_SIZE_X * WORLD_CHUNKS_X;
	const int WORLD_SIZE_Z = CHUNK_SIZE_Z * WORLD_CHUNKS_Z;
	const int WORLD_SIZE_Y = CHUNK_SIZE_Y;

	const int FACE_DATA_LENGTH = 66;
	const int CHUNK_DATA_LENGTH = CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * 6 * FACE_DATA_LENGTH;

	const int SUNLIGHT_LEVEL_MAX = 16;

	const fvec3 adjacent[6] = { fvec3{ 0,-1,0 }, fvec3{ 0,1,0 },fvec3{ 1,0,0 },fvec3{ -1,0,0 },fvec3{ 0,0,-1 },fvec3{ 0,0,1 } };
	const int adjacent_opposite[6] = { 1,0,3,2,5,4};
	const ivec2 adjacent_chunk[6] = { ivec2{ 0,-1}, ivec2{ 0,1},ivec2{ 1,0},ivec2{ -1,0} };
}


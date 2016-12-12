#include "Chunk.h"
#include "World.h"

Chunk::Chunk(World* worldPointer, Perlin *p, ivec2 coords, Block* blockList, GraphicsManager *graphicsManager, int shaderId)
{
	chunkCoords = coords;
	blocks = blockList;
	graphics = graphicsManager;

	shader = shaderId;
	bufferResourceId = graphics->newBuffer(shader);

	world = worldPointer;
	perlin = p;

    state = ChunkState::OutOfRange;
	meshDirty = false;


	meshLength = 0;
}


Chunk::~Chunk()
{
	graphics->deleteBuffer(bufferResourceId);
}

void Chunk::loadData()
{
	genChunk();
}

void Chunk::genChunk() {
	Perlin p2(6, 4, 1, 50);
	Perlin p3(1, 1, 2, 59);
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int z = 0; z < CHUNK_SIZE_Z; z++) {
			int stoneHeight = 60 + 9 * perlin->Get((x + chunkCoords.x * CHUNK_SIZE_X) * 0.003f, (z + chunkCoords.z* CHUNK_SIZE_Z)*0.003f) + 50 * p2.Get((x + chunkCoords.x * CHUNK_SIZE_X) * 0.0003f, (z + chunkCoords.z* CHUNK_SIZE_Z)*0.0003f);
			for (int y = 0; y < stoneHeight; y++) {
				data[x][z][y].blockId = 1;
			}
			int caveHeight = std::max(10 * p3.Get((x + chunkCoords.x * CHUNK_SIZE_X) * 0.03f, (z + chunkCoords.z* CHUNK_SIZE_Z)*0.03f) ,0.0f);
			for (int y = stoneHeight - caveHeight; y < stoneHeight + caveHeight; y++) {
				data[x][z][y].blockId = 0;
			}

			int dirtHeight = 10 + 5 * perlin->Get((x + chunkCoords.x * CHUNK_SIZE_X)*0.003f, (z + chunkCoords.z* CHUNK_SIZE_Z)*0.003f);
			for (int y = stoneHeight; y < stoneHeight + dirtHeight; y++) {
				data[x][z][y].blockId = 2;
			}
			data[x][z][dirtHeight + stoneHeight].blockId = 3;
			for (int y = dirtHeight + stoneHeight + 1; y < CHUNK_SIZE_Y; y++) {
				data[x][z][y].blockId = 0;
			}
		}
	}

	std::queue<lightPropogationNode> lightQueue;
	for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; ++x) {
		for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1) * CHUNK_SIZE_Z; ++z) {
			int y = CHUNK_SIZE_Y - 1;

			setSunlightLevel({ x, z, y }, SUNLIGHT_LEVEL_MAX);
			lightQueue.push({ {x,z,y}, SUNLIGHT_LEVEL_MAX });
		}
	}

	while (!lightQueue.empty()) {
		lightPropogationNode n = lightQueue.front();
		lightQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; ++i) {
			newPos = n.position + adjecent[i];
			unsigned short newLight = getSunlightLevel(newPos);
			if (i != 4) {
				if (getBlock(newPos) == 0 && newLight < n.lightLevel - 1) {
					setSunlightLevel(newPos, n.lightLevel - 1);
					lightQueue.push({ newPos, n.lightLevel - 1 });
				}
			}
			else if (getBlock(newPos) == 0 && newLight < n.lightLevel) {
				setSunlightLevel(newPos, n.lightLevel);
				lightQueue.push({ newPos, n.lightLevel });

			}
		}
	}


	if (getBlock({ (chunkCoords.x * CHUNK_SIZE_X) - 1, chunkCoords.z * CHUNK_SIZE_Z, 0 }) > -1) {
		for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1) * CHUNK_SIZE_Z; ++z) {
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; --y) {
				int x = chunkCoords.x * CHUNK_SIZE_X;

				fvec3 newPos = fvec3{ x,z,y } +adjecent[3];
				if (getBlock(newPos) == 0 && getBlock({ x,z,y }) == 0) {
					unsigned short newLight = getSunlightLevel(newPos);
					unsigned short oldLight = getSunlightLevel({ x,z,y });
					if (oldLight < newLight - 1) {
						lightQueue.push({ newPos, newLight });
					}
					else if (newLight < oldLight - 1) {
						lightQueue.push({ { x,z,y }, oldLight });
					}
				}
			}
		}
	}
	if (getBlock({ (chunkCoords.x + 1) * CHUNK_SIZE_X, chunkCoords.z * CHUNK_SIZE_Z, 0 }) > -1) {
		for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1) * CHUNK_SIZE_Z; ++z) {
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; --y) {
				int x = (chunkCoords.x + 1) * CHUNK_SIZE_X - 1;

				fvec3 newPos = fvec3{ x,z,y } +adjecent[2];
				if (getBlock(newPos) == 0 && getBlock({ x,z,y }) == 0) {
					unsigned short newLight = getSunlightLevel(newPos);
					unsigned short oldLight = getSunlightLevel({ x,z,y });
					if (oldLight < newLight - 1) {
						lightQueue.push({ newPos, newLight });
					}
					else if (newLight < oldLight - 1) {
						lightQueue.push({ { x,z,y }, oldLight });
					}
				}
			}
		}
	}
	if (getBlock({ chunkCoords.x * CHUNK_SIZE_X, (chunkCoords.z * CHUNK_SIZE_Z) - 1, 0 }) > -1) {
		for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; ++x) {
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; --y) {
				int z = chunkCoords.z * CHUNK_SIZE_Z;

				fvec3 newPos = fvec3{ x,z,y } +adjecent[0];
				if (getBlock(newPos) == 0 && getBlock({ x,z,y }) == 0) {
					unsigned short newLight = getSunlightLevel(newPos);
					unsigned short oldLight = getSunlightLevel({ x,z,y });
					if (oldLight < newLight - 1) {
						lightQueue.push({ newPos, newLight });
					}
					else if (newLight < oldLight - 1) {
						lightQueue.push({ { x,z,y }, oldLight });
					}
				}
			}
		}
	}
	if (getBlock({ chunkCoords.x * CHUNK_SIZE_X, (chunkCoords.z + 1) * CHUNK_SIZE_Z, 0 }) > -1) {
		for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; ++x) {
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; --y) {
				int z = (chunkCoords.z + 1) * CHUNK_SIZE_Z - 1;

				fvec3 newPos = fvec3{ x,z,y } +adjecent[1];
				if (getBlock(newPos) == 0 && getBlock({ x,z,y }) == 0) {
					unsigned short newLight = getSunlightLevel(newPos);
					unsigned short oldLight = getSunlightLevel({ x,z,y });
					if (oldLight < newLight - 1) {
						lightQueue.push({ newPos, newLight});
					}
					else if (newLight < oldLight -3) {
						lightQueue.push({ { x,z,y }, oldLight });
					}
				}
			}
		}
	}
	while (!lightQueue.empty()) {
		lightPropogationNode n = lightQueue.front();
		lightQueue.pop();


		fvec3 newPos;
		for (int i = 0; i < 6; ++i) {
			newPos = n.position + adjecent[i];
			unsigned short newLight = getSunlightLevel(newPos);
			if (i != 4) {
				if (getBlock(newPos) == 0 && newLight < n.lightLevel - 1) {
					setSunlightLevel(newPos, n.lightLevel - 1);
					lightQueue.push({ newPos, n.lightLevel - 1 });
				}
			}
			else if (getBlock(newPos) == 0 && newLight < n.lightLevel) {
				setSunlightLevel(newPos, n.lightLevel);
				lightQueue.push({ newPos, n.lightLevel });

			}
		}
	}
}

float * Chunk::genMesh() {
	mesh = new float[CHUNK_DATA_LENGTH];
	int k = 0;
	for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; x++) {
		for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1)* CHUNK_SIZE_Z; z++) {
			for (int y = 0; y < CHUNK_SIZE_Y; y++) {
				int id = getBlock({ x,z,y });
				if (id > 0) {
					if (getBlock({ x, z - 1, y }) < 1) {
						genFace(0, x, z, y, blocks[id].textures[0].texId, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z + 1, y }) < 1) {
						genFace(1, x, z, y, blocks[id].textures[1].texId, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x + 1, z, y }) < 1) {
						genFace(2, x, z, y, blocks[id].textures[2].texId, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x - 1, z, y }) < 1) {
						genFace(3, x, z, y, blocks[id].textures[3].texId, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z, y - 1 }) < 1) {
						genFace(4, x, z, y, blocks[id].textures[4].texId, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z, y + 1 }) < 1) {
						genFace(5, x, z, y, blocks[id].textures[5].texId, k);
						k += FACE_DATA_LENGTH;
					}
				}
			}
		}
	}

	tempMeshLength = k;

	return mesh;
}

float * Chunk::genNaiveMesh() {

	/*mesh = new float[CHUNK_DATA_LENGTH]();
	int k = 0;
	for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; x++) {
		for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1)* CHUNK_SIZE_Z; z++) {
			for (int y = 0; y < CHUNK_SIZE_Y; y++) {
				int id = getBlock({ x,z,y });
				if (id > 0) {
					if (getBlock({ x, z - 1, y }) < 1) {
						genFace(0, x, z, y, blocks[id].uvs[0].u, blocks[id].uvs[0].v, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z + 1, y }) < 1) {
						genFace(1, x, z, y, blocks[id].uvs[1].u, blocks[id].uvs[1].v, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x + 1, z, y }) < 1) {
						genFace(2, x, z, y, blocks[id].uvs[2].u, blocks[id].uvs[2].v, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x - 1, z, y }) < 1) {
						genFace(3, x, z, y, blocks[id].uvs[3].u, blocks[id].uvs[3].v, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z, y - 1 }) < 1) {
						genFace(4, x, z, y, blocks[id].uvs[4].u, blocks[id].uvs[4].v, k);
						k += FACE_DATA_LENGTH;
					}
					if (getBlock({ x, z, y + 1 }) < 1) {
						genFace(5, x, z, y, blocks[id].uvs[5].u, blocks[id].uvs[5].v, k);
						k += FACE_DATA_LENGTH;
					}
				}
				else {
					k += FACE_DATA_LENGTH;
				}
			}
		}
	}

	meshLength = k;
	*/
	return mesh;
}



void Chunk::genFace(int side, int x, int z, int y, int arrayTexId, int delta) {
	int sideDelta = side * FACE_DATA_LENGTH;
	const float *blockMesh = cube;

	int vertAos[6];
	for (int vert = 0; vert > 6; vert += 1) {
		int i = vert * 9;
		vertAos[vert] = vertexOccluders({ x,z,y }, blockMesh[sideDelta + i], blockMesh[sideDelta + i + 2], blockMesh[sideDelta + i + 1], side);
	}

	if (vertAos[0] + vertAos[2] < vertAos[1] + vertAos[5])
		blockMesh = cubealt;
	else
		blockMesh = cube;

	int i = 0;
	for (int vert = 0; vert < 6; vert += 1) {

		mesh[delta + i] = blockMesh[sideDelta + i] + x;
		mesh[delta + i + 1] = blockMesh[sideDelta + i + 1] + y;
		mesh[delta + i + 2] = blockMesh[sideDelta + i + 2] + z;

		int aoLevel = vertexOccluders({ x,z,y }, blockMesh[sideDelta + i], blockMesh[sideDelta + i + 2], blockMesh[sideDelta + i + 1], side);

		for (int k = i + 3; k < i + 6; k++) {
			mesh[delta + k] = getLightLevel(fvec3{ x,z,y } + adjecent[side]) * 0.1f;
		}

		mesh[delta + i + 6] = getSunlightLevel(fvec3{ x,z,y } + adjecent[side]);

		mesh[delta + i + 7] = aoLevel;

        mesh[delta + i + 8] = blockMesh[sideDelta + i + 6];
        mesh[delta + i + 9] = blockMesh[sideDelta + i + 7];

		mesh[delta + i + 10] = arrayTexId;

		i += 11;
	}

}

int Chunk::vertexOccluders(fvec3 position, float x, float z, float y, int side)
{
	int xSide = (x > 0) ? 1 : -1;
	int ySide = (y > 0) ? 1 : -1;
	int zSide = (z > 0) ? 1 : -1;

	int side1;
	int side2;
	int corner;

	if (side > 3) {
		side1 = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
		side2 = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;
		corner = (getBlock(position + fvec3(xSide, zSide, ySide)) > 0) ? 1 : 0;
	}
	else if (side < 2) {
		side1 = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
		side2 = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;
		corner = (getBlock(position + fvec3(xSide, zSide, ySide)) > 0) ? 1 : 0;
	}
	else {
		side1 = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
		side2 = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
		corner = (getBlock(position + fvec3(xSide, zSide, ySide)) > 0) ? 1 : 0;
	}

	if (side1 && side2)
		return 0;
	return 3 - (side1 + side2 + corner);
}

fvec3 Chunk::getRelativePosition(fvec3 worldPosition)
{
	return worldPosition - (fvec3(chunkCoords.x, chunkCoords.z, 0) * CHUNK_SIZE);
}

fvec3 Chunk::getWorldPosition(fvec3 relativePosition)
{
	return relativePosition + (fvec3(chunkCoords.x, chunkCoords.z, 0) * CHUNK_SIZE);
}

float Chunk::nearestBound(float pos, float speed)
{
	if (speed < 0)
		return nearestBound(-pos, -speed) + 0.00001;
	else
		return (floor(pos) + (speed > 0 ? 1 : 0) - pos) / speed;
}

bool Chunk::raycast(fvec3 origin, fvec3 direction, float distance, RayCollision *ray)
{
	float t = ray->distance;
	fvec3 position = origin + direction * t;
	ray->side = adjecent_opposite[ray->side];
	while (t < distance) {
		position = origin + direction * t;

		if (position.x >= CHUNK_SIZE_X * (chunkCoords.x + 1)) {
			ray->side = 2;
			return false;
		}
		else if (position.x < (CHUNK_SIZE_X * chunkCoords.x)) {
			ray->side = 3;
			return false;
		}
		else if (position.z >= CHUNK_SIZE_Z + (CHUNK_SIZE_Z * chunkCoords.z)) {
			ray->side = 1;
			return false;
		}
		else if (position.z < (CHUNK_SIZE_Z * chunkCoords.z)) {
			ray->side = 0;
			return false;
		}
		else if (position.y >= CHUNK_SIZE_Y) {
			ray->side = 5;
			return false;
		}
		else if (position.y < 0) {
			ray->side = 4;
			return false;
		}

		if (getBlock(position) > 0) {
			ray->isCollision = true;

			ray->blockPos = position;
			ray->chunk = chunkCoords;
			return true;
		}

		float xT = nearestBound(position.x, direction.x);
		float zT = nearestBound(position.z, direction.z);
		float yT = nearestBound(position.y, direction.y);

		if (xT < zT) {
			if (xT < yT) {
				t += xT;
				if (direction.x > 0)
					ray->side = 3;
				else
					ray->side = 2;
			}
			else {
				t += yT;
				if (direction.y > 0)
					ray->side = 4;
				else
					ray->side = 5;
			}
		}
		else {
			if (zT < yT) {
				t += zT;

				if (direction.z > 0)
					ray->side = 0;
				else
					ray->side = 1;
			}
			else {
				t += yT;
				if (direction.y > 0)
					ray->side = 4;
				else
					ray->side = 5;
			}
		}

		ray->distance = t;

	}
	return false;
}

void Chunk::removeBlock(fvec3 position)
{
	if (getBlock({ position.x, position.z, position.y }) > 0) {
		setBlock(position, 0);
		//genMesh();
		meshDirty = true;
	}
}

void Chunk::addBlock(fvec3 position, int blockId)
{
	if (!getBlock(position)) {
		setBlock(position, blockId);
		//genMesh();
		meshDirty = true;
	}
}

unsigned short Chunk::getLightLevel(fvec3 worldPosition)
{
	fvec3 rel = getRelativePosition(worldPosition);
	if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y)
		return 0;
	else if (rel.x < 0 || rel.z < 0 || rel.z >= CHUNK_SIZE_Z || rel.x >= CHUNK_SIZE_X) {
		return world->getLightLevel(worldPosition);
	}
	else
		return data[(int)floor(rel.x)][(int)floor(rel.z)][(int)floor(rel.y)].lightLevel;
}

void Chunk::setLightLevel(fvec3 worldPosition, unsigned short lightLevel)
{
	fvec3 position = getRelativePosition(worldPosition);
	if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
		return;
	data[(int)position.x][(int)position.z][(int)position.y].lightLevel = lightLevel;
	meshDirty = true;
}

unsigned short Chunk::getSunlightLevel(fvec3 worldPosition) {
	fvec3 rel = getRelativePosition(worldPosition);
	if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y)
		return 0;
	else if (rel.x < 0 || rel.z < 0 || rel.z >= CHUNK_SIZE_Z || rel.x >= CHUNK_SIZE_X) {
		return world->getSunlightLevel(worldPosition);
	}
	else
		return data[(int)floor(rel.x)][(int)floor(rel.z)][(int)floor(rel.y)].sunlightLevel;
}

void Chunk::setSunlightLevel(fvec3 worldPosition, unsigned short lightLevel)
{
	fvec3 position = getRelativePosition(worldPosition);
	if (position.y >= CHUNK_SIZE_Y || position.y < 0)
		return;
	if (position.x < 0 || position.z < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z) {
		world->setSunlightLevel(worldPosition, lightLevel);
		return;
	}
	data[(int)floor(position.x)][(int)floor(position.z)][(int)floor(position.y)].sunlightLevel = lightLevel;
	meshDirty = true;
}

void Chunk::updateBuffer() {
	graphics->updateBuffer(bufferResourceId, mesh, meshLength);
	delete[] mesh;
}

void Chunk::render()
{
	if (state >= ChunkState::Loaded)
		graphics->renderBuffer(bufferResourceId, meshLength / 11);

}

bool Chunk::checkCollision(fvec3 point) {
	if (getBlock(floor(point)) > 0)
		return true;
	return false;
}

bool Chunk::checkCollision(AABB box) {
	fvec3 pos = getRelativePosition(box.getPosition());
	fvec3 bound = pos + box.getSize();

	for (int i = pos.x; i < bound.x; i++) {
		for (int j = pos.z; j < bound.z; j++) {
			for (int k = pos.y; k < bound.y; k++) {
				if (getBlockRelative({ i, j, k }) > 0)
					return true;
			}
		}
	}
	return false;
}

int Chunk::getBlock(fvec3 worldPosition) {
	fvec3 rel = getRelativePosition(worldPosition);
	if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y)
		return -1;
	else if (rel.x < 0 || rel.z < 0 || rel.z >= CHUNK_SIZE_Z || rel.x >= CHUNK_SIZE_X) {
		return world->getBlock(worldPosition);
	}
	else
		return data[(int)floor(rel.x)][(int)floor(rel.z)][(int)floor(rel.y)].blockId;
}

int Chunk::getBlockRelative(fvec3 position) {
	if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
		return -1;
	else
		return data[(int)position.x][(int)position.z][(int)position.y].blockId;
}

void Chunk::setBlock(fvec3 worldPosition, int blockId)
{
	fvec3 position = getRelativePosition(worldPosition);
	if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
		return;
	data[(int)position.x][(int)position.z][(int)position.y].blockId = blockId;
	meshDirty = true;
}

void Chunk::setBlockRelative(fvec3 position, int blockId)
{
	if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
		return;
	data[(int)position.x][(int)position.z][(int)position.y].blockId = blockId;
}

bool Chunk::isLoaded() {
	return state >= ChunkState::Loaded;
}

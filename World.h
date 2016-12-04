#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <list>

#include "Const.h"
#include "Types.h"

#include "GraphicsManager.h"
#include "Chunk.h"
#include "Block.h"
#include "Perlin.h"
#include "Box.h"

class World
{
public:
	World(GraphicsManager *graphicsManager, int shaderId);
	~World();
	Block blocks[5];

	void genChunks(Perlin *p);
	RayCollision raycast(fvec3 position, fvec3 direction, float distance);
	bool checkCollision(AABB box);
	bool checkCollision(BoundingBox box);

	float * getMesh(ivec2 coords);

	int getBlock(fvec3 position);
	void setBlock(fvec3 position, int blockId);
	void removeBlock(fvec3 position);
	void addBlock(fvec3 position, int blockId);
	void setLightLevel(fvec3 position, unsigned short lightLevel);
	unsigned short getLightLevel(fvec3 position);
	void setSunlightLevel(fvec3 position, unsigned short lightLevel);
	unsigned short getSunlightLevel(fvec3 position);

	void update(fvec3 playerPos, float elapsedTime);
	void renderChunks();

	void createBoxes(int shaderId);
	void createRandomBoxes(int shaderId);
	void renderBoxes(glm::mat4 *PV);
	
private:
	//Chunk *chunks[WORLD_CHUNKS_X][WORLD_CHUNKS_Z];
	std::unordered_map<ivec2, Chunk*> chunks;

	GraphicsManager *graphics;
	int shader;

	Chunk* getChunk(ivec2 coords);
	ivec2 getChunkCoords(fvec3 worldPosition);
	ivec2 getChunkCoords(ivec2 worldPosition);

	Perlin *perlin;

	std::mutex m;
	std::condition_variable meshWaiter;
	std::condition_variable loadWaiter;
	std::priority_queue<Chunk*, std::vector<Chunk*>, Chunk::Comparator> meshJobs;
	std::priority_queue<Chunk*, std::vector<Chunk*>, Chunk::Comparator> loadJobs;
    void addCompleteJobToQueue(std::queue<Chunk*> *q, Chunk* job);
    std::queue<Chunk*> finishedMeshJobs;
    std::queue<Chunk*> finishedLoadJobs;
	void meshTask();
	void loadTask();
	Chunk* getNextMeshJob();
	Chunk* getNextLoadJob();

	bool isChunkInLoadRange(ivec2 chunkCoords, ivec2 playerCoords);
	bool isChunkInMeshRange(ivec2 chunkCoords, ivec2 playerCoords);
	bool isChunkNeighborsLoaded(ivec2 chunkCoords);

	void addLight(fvec3 position, int lightLevel);
	void removeLight(fvec3 position);

	std::mutex physMutex;
	std::condition_variable physWaiter;
	std::list<Box*> boxes;
	void physTask();
	Box *getNextPhysObj();
};


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
#include "simplexnoise.h"
#include "Box.h"

class World {
public:
    World(GraphicsManager *graphicsManager, int shaderId);
    ~World();
    Block blocks[15];

    void genChunks(Perlin *p);
    RayCollision raycast(fvec3 position, fvec3 direction, float distance);
    bool checkCollision(AABB box);
    bool checkCollision(BoundingBox box);

    int getBlock(fvec3 position);
    void setBlock(fvec3 position, int blockId);
    void removeBlock(fvec3 position);
    void addBlock(fvec3 position, int blockId);
    void setLightLevel(fvec3 position, unsigned short lightLevel);
    LightLevel getLightLevel(fvec3 position);
    void setSunlightLevel(fvec3 position, unsigned short lightLevel);
    LightLevel getSunlightLevel(fvec3 position);

    void update(fvec3 playerPos, float elapsedTime);
    void renderChunks();

    void createBoxes(int shaderId);
    void createRandomBoxes(int shaderId);
    void renderBoxes(glm::mat4 *PV);


private:
    std::unordered_map<ivec2, Chunk *> chunks;

    Perlin *perlin;
    GraphicsManager *graphics;
    int shader;

    Chunk *getChunk(ivec2 coords);
    ivec2 getChunkCoords(fvec3 worldPosition);
    ivec2 getChunkCoords(ivec2 worldPosition);

    void genChunk(Chunk *chunk);

    std::mutex m;
    std::condition_variable meshWaiter;
    std::condition_variable loadWaiter;

    ivec2 chunkLoadCenter;
    std::list<Chunk *> meshJobs;
    std::list<Chunk *> loadJobs;
    Chunk *getNextJob(std::list<Chunk *> *jobs);
    void addCompleteJobToQueue(std::queue<Chunk *> *q, Chunk *job);
    std::queue<Chunk *> finishedMeshJobs;
    std::queue<Chunk *> finishedLoadJobs;
    void meshTask();
    void loadTask();
    Chunk *getNextMeshJob();
    Chunk *getNextLoadJob();

    bool isChunkInLoadRange(ivec2 chunkCoords, ivec2 playerCoords);
    bool isChunkInMeshRange(ivec2 chunkCoords, ivec2 playerCoords);
    bool isChunkNeighborsLoaded(ivec2 chunkCoords);

    void addLight(fvec3 position, LightLevel lightLevel, int channel);
    void removeLight(fvec3 position, int channel);
    void removeSunlight(fvec3 position);

    std::mutex physMutex;
    std::condition_variable physWaiter;
    std::list<Box *> boxes;
    void physTask();
    Box *getNextPhysObj();

    int flattenVector(fvec3 coords);
};


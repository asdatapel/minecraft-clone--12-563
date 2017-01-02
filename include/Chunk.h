#pragma once

#include <iostream>
#include <algorithm>
#include <math.h>
#include <stack>

#include "Types.h"
#include "Const.h"
#include "GraphicsManager.h"
#include "Perlin.h"
#include "Block.h"

#include <glm/glm.hpp>

using namespace Const;

class World;

enum ChunkState {
    OutOfRange = 0,
    InLoadingQueue,
    CurrentlyLoading,
    Loaded,
    InMeshingQueue,
    CurrentlyMeshing,
    Meshed
};

struct Block1 {
    unsigned int blockId;
    //LightLevel lightLevel;
    LightLevel lightLevel[3];
    LightLevel sunlightLevel;
};

class Chunk {
public:
    Chunk(World *world, Perlin *p, ivec2 coords, Block *blockList, GraphicsManager *graphicsManager, int shaderId);

    ~Chunk();

    void loadData();
    void genChunk();
    float *genMesh();

    Chunk *neighbors[4];

    bool isLoaded();
    int flattenVector(fvec3 coords);

    bool checkCollision(fvec3 pos);
    bool checkCollision(AABB box);
    bool raycast(fvec3 position, fvec3 direction, float distance, RayCollision *ray);

    int getBlock(fvec3 position);
    int getBlockRelative(fvec3 position);
    void setBlock(fvec3 position, unsigned int blockId);
    void removeBlock(fvec3 position);
    void addBlock(fvec3 position, int blockId);

    void addLight(fvec3 position, LightLevel lightLevel, int channel);
    void removeLight(fvec3 position, int channel);
    void addSunlight(fvec3 position, LightLevel lightLevel);
    void removeSunlight(fvec3 position);
    void updateLight(fvec3 position, int channel);
    void updateSunlight(fvec3 position);

    LightLevel getLightLevel(fvec3 worldPosition, int channel);
    void setLightLevel(fvec3 worldPosition, LightLevel lightLevel, int channel);
    LightLevel getSunlightLevel(fvec3 worldPosition);
    void setSunlightLevel(fvec3 worldPosition, LightLevel lightLevel);

    void updateBuffer();
    void render();

    ChunkState state;
    bool meshDirty;

    int meshLength;
    int tempMeshLength;

//private:
    World *world;
    Perlin *perlin;
    ivec2 chunkCoords;

    Chunk* getChunkRelative(fvec3 relPosition);
    Chunk* getChunk(fvec3 relPosition);

    void nullifyLight(fvec3 position, int channel);
    void fillLight(fvec3 position, LightLevel lightLevel, int channel);
    void nullifySunlight(fvec3 position);
    void fillSunlight(fvec3 position, LightLevel lightLevel);

    void addSunlightLocal(fvec3 position, LightLevel lightLevel);


    Block1 *data;
    float *mesh;

    Block *blocks;
    GraphicsManager *graphics;
    int bufferResourceId;
    int shader;

    void genFace(int side, int x, int z, int y, int arrayTexId, int delta);
    int vertexOccluders(fvec3 position, float x, float z, float y, int side);
    LightLevel getVertexLight(fvec3 position, float x, float z, float y, int side, int channel);
    LightLevel getVertexSunlight(fvec3 position, float x, float z, float y, int side);
    float nearestBound(float pos, float speed);

    fvec3 getRelativePosition(fvec3 worldPosition);

    const float *cube[6] = {
            side0, side1, side2, side3, side4, side5
    };

    //the order in which the vertices are drawn
    const int faces[6] = {
            0, 1, 2, 3, 2, 1,
    };
    //same as faces, except with the triangles flipped
    const int alt_faces[6] = {
            0, 1, 3, 0, 3, 2,
    };

    const float side0[20] = {
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    const float side1[20] = {
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };
    const float side2[20] = {
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    const float side3[20] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };
    const float side4[20] = {
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    const float side5[20] = {
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
};

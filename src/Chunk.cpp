#include "Chunk.h"
#include "World.h"

Chunk::Chunk(World *worldPointer, Perlin *p, ivec2 coords, Block *blockList, GraphicsManager *graphicsManager,
             int shaderId) {
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

    data = new Block1[16 * 16 * 256];
}


Chunk::~Chunk() {
    graphics->deleteBuffer(bufferResourceId);
}

void Chunk::loadData() {
    genChunk();
}

void Chunk::genChunk() {

}

float *Chunk::genMesh() {
    mesh = new float[CHUNK_DATA_LENGTH];
    int k = 0;
    for (int x = chunkCoords.x * CHUNK_SIZE_X; x < (chunkCoords.x + 1) * CHUNK_SIZE_X; x++) {
        for (int z = chunkCoords.z * CHUNK_SIZE_Z; z < (chunkCoords.z + 1) * CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                int id = getBlock({x, z, y});
                if (id > 0) {
                    if (getBlock({x, z - 1, y}) < 1) {
                        genFace(0, x, z, y, blocks[id].textures[0].texId, k);
                        k += FACE_DATA_LENGTH;
                    }
                    if (getBlock({x, z + 1, y}) < 1) {
                        genFace(1, x, z, y, blocks[id].textures[1].texId, k);
                        k += FACE_DATA_LENGTH;
                    }
                    if (getBlock({x + 1, z, y}) < 1) {
                        genFace(2, x, z, y, blocks[id].textures[2].texId, k);
                        k += FACE_DATA_LENGTH;
                    }
                    if (getBlock({x - 1, z, y}) < 1) {
                        genFace(3, x, z, y, blocks[id].textures[3].texId, k);
                        k += FACE_DATA_LENGTH;
                    }
                    if (getBlock({x, z, y - 1}) < 1) {
                        genFace(4, x, z, y, blocks[id].textures[4].texId, k);
                        k += FACE_DATA_LENGTH;
                    }
                    if (getBlock({x, z, y + 1}) < 1) {
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

void Chunk::genFace(int side, int x, int z, int y, int arrayTexId, int delta) {
    const float *sideMesh = cube[side];

    LightLevel sunlight[4];
    LightLevel lighting[4];
    LightLevel lighting1[4];
    LightLevel lighting2[4];
    for (int vert = 0; vert < 4; ++vert) {
        int i = vert * 5;
        sunlight[vert] = getVertexSunlight({x, z, y}, sideMesh[i], sideMesh[i + 2], sideMesh[i + 1], side);
        lighting[vert] = getVertexLight({x, z, y}, sideMesh[i], sideMesh[i + 2], sideMesh[i + 1], side, 0);
        lighting1[vert] = getVertexLight({x, z, y}, sideMesh[i], sideMesh[i + 2], sideMesh[i + 1], side, 1);
        lighting2[vert] = getVertexLight({x, z, y}, sideMesh[i], sideMesh[i + 2], sideMesh[i + 1], side, 2);
    }

    const int *vert_order = faces;
    if (sunlight[0] + sunlight[3] < sunlight[1] + sunlight[2])
        vert_order = faces;
    else
        vert_order = alt_faces;

    int i = 0;
    for (int vert = 0; vert < 6; vert += 1) {

        int vertexDelta = vert_order[vert] * 5;

        mesh[delta + i] = sideMesh[vertexDelta] + x;
        mesh[delta + i + 1] = sideMesh[vertexDelta + 1] + y;
        mesh[delta + i + 2] = sideMesh[vertexDelta + 2] + z;

        mesh[delta + i + 3] = lighting[vert_order[vert]] * 0.05f;
        mesh[delta + i + 4] = lighting1[vert_order[vert]] * 0.05f;
        mesh[delta + i + 5] = lighting2[vert_order[vert]] * 0.05f;

        mesh[delta + i + 6] = sunlight[vert_order[vert]] * 0.5f;

        mesh[delta + i + 7] = 1;

        mesh[delta + i + 8] = sideMesh[vertexDelta + 3];
        mesh[delta + i + 9] = sideMesh[vertexDelta + 4];

        mesh[delta + i + 10] = arrayTexId;

        i += FACE_DATA_LENGTH / 6;
    }
}

int Chunk::vertexOccluders(fvec3 position, float x, float z, float y, int side) {
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
    } else if (side < 2) {
        side1 = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
        side2 = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;
        corner = (getBlock(position + fvec3(xSide, zSide, ySide)) > 0) ? 1 : 0;
    } else {
        side1 = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
        side2 = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
        corner = (getBlock(position + fvec3(xSide, zSide, ySide)) > 0) ? 1 : 0;
    }

    if (side1 && side2)
        return 0;
    return 3 - (side1 + side2 + corner);
}

LightLevel Chunk::getVertexLight(fvec3 position, float x, float z, float y, int side, int channel) {
    int xSide = (x > 0) ? 1 : -1;
    int ySide = (y > 0) ? 1 : -1;
    int zSide = (z > 0) ? 1 : -1;

    int side1Block;
    int side2Block;

    LightLevel side1;
    LightLevel side2;
    LightLevel top;
    LightLevel corner;

    if (side > 3) {
        side1Block = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;

        side1 = getLightLevel(position + fvec3(xSide, 0, ySide), channel);
        side2 = getLightLevel(position + fvec3(0, zSide, ySide), channel);
        top = getLightLevel(position + fvec3(0, 0, ySide), channel);
        corner = getLightLevel(position + fvec3(xSide, zSide, ySide), channel);
    } else if (side < 2) {
        side1Block = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;

        side1 = getLightLevel(position + fvec3(xSide, zSide, 0), channel);
        side2 = getLightLevel(position + fvec3(0, zSide, ySide), channel);
        top = getLightLevel(position + fvec3(0, zSide, 0), channel);
        corner = getLightLevel(position + fvec3(xSide, zSide, ySide), channel);
    } else {
        side1Block = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;

        side1 = getLightLevel(position + fvec3(xSide, 0, ySide), channel);
        side2 = getLightLevel(position + fvec3(xSide, zSide, 0), channel);
        top = getLightLevel(position + fvec3(xSide, 0, 0), channel);
        corner = getLightLevel(position + fvec3(xSide, zSide, ySide), channel);
    }

    if (side1Block && side2Block)
        return ((side1 > 0) ? side1 : (LightLevel) 0) + ((side2 > 0) ? side2 : (LightLevel) 0) +
               ((top > 0) ? top : (LightLevel) 0);
    return ((side1 > 0) ? side1 : (LightLevel) 0) + ((side2 > 0) ? side2 : (LightLevel) 0) +
           ((corner > 0) ? corner : (LightLevel) 0) + ((top > 0) ? top : (LightLevel) 0);
}

LightLevel Chunk::getVertexSunlight(fvec3 position, float x, float z, float y, int side) {
    int xSide = (x > 0) ? 1 : -1;
    int ySide = (y > 0) ? 1 : -1;
    int zSide = (z > 0) ? 1 : -1;

    int side1Block;
    int side2Block;

    LightLevel side1;
    LightLevel side2;
    LightLevel top;
    LightLevel corner;

    if (side > 3) {
        side1Block = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;

        side1 = getSunlightLevel(position + fvec3(xSide, 0, ySide));
        side2 = getSunlightLevel(position + fvec3(0, zSide, ySide));
        top = getSunlightLevel(position + fvec3(0, 0, ySide));
        corner = getSunlightLevel(position + fvec3(xSide, zSide, ySide));
    } else if (side < 2) {
        side1Block = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(0, zSide, ySide)) > 0) ? 1 : 0;

        side1 = getSunlightLevel(position + fvec3(xSide, zSide, 0));
        side2 = getSunlightLevel(position + fvec3(0, zSide, ySide));
        top = getSunlightLevel(position + fvec3(0, zSide, 0));
        corner = getSunlightLevel(position + fvec3(xSide, zSide, ySide));
    } else {
        side1Block = (getBlock(position + fvec3(xSide, 0, ySide)) > 0) ? 1 : 0;
        side2Block = (getBlock(position + fvec3(xSide, zSide, 0)) > 0) ? 1 : 0;

        side1 = getSunlightLevel(position + fvec3(xSide, 0, ySide));
        side2 = getSunlightLevel(position + fvec3(xSide, zSide, 0));
        top = getSunlightLevel(position + fvec3(xSide, 0, 0));
        corner = getSunlightLevel(position + fvec3(xSide, zSide, ySide));
    }

    if (side1Block && side2Block)
        return ((side1 > 0) ? side1 : (LightLevel) 0) + ((side2 > 0) ? side2 : (LightLevel) 0) +
               ((top > 0) ? top : (LightLevel) 0);
    return ((side1 > 0) ? side1 : (LightLevel) 0) + ((side2 > 0) ? side2 : (LightLevel) 0) +
           ((corner > 0) ? corner : (LightLevel) 0) + ((top > 0) ? top : (LightLevel) 0);
}

bool Chunk::raycast(fvec3 origin, fvec3 direction, float distance, RayCollision *ray) {
    float t = ray->distance;
    fvec3 position = origin + direction * t;
    ray->side = adjacent_opposite[ray->side];
    while (t < distance) {
        position = origin + direction * t;

        if (position.x >= CHUNK_SIZE_X * (chunkCoords.x + 1)) {
            ray->side = 2;
            return false;
        } else if (position.x < (CHUNK_SIZE_X * chunkCoords.x)) {
            ray->side = 3;
            return false;
        } else if (position.z >= CHUNK_SIZE_Z + (CHUNK_SIZE_Z * chunkCoords.z)) {
            ray->side = 1;
            return false;
        } else if (position.z < (CHUNK_SIZE_Z * chunkCoords.z)) {
            ray->side = 0;
            return false;
        } else if (position.y >= CHUNK_SIZE_Y) {
            ray->side = 5;
            return false;
        } else if (position.y < 0) {
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
            } else {
                t += yT;
                if (direction.y > 0)
                    ray->side = 4;
                else
                    ray->side = 5;
            }
        } else {
            if (zT < yT) {
                t += zT;

                if (direction.z > 0)
                    ray->side = 0;
                else
                    ray->side = 1;
            } else {
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

float Chunk::nearestBound(float pos, float speed) {
    if (speed < 0)
        return nearestBound(-pos, -speed) + 0.00001;
    else
        return (floor(pos) + (speed > 0 ? 1 : 0) - pos) / speed;
}

void Chunk::addLight(fvec3 position, LightLevel lightLevel, int channel) {
    std::queue<lightPropogationNode> lightQueue;

    data[flattenVector(getRelativePosition(position))].lightLevel[channel] = lightLevel;
    meshDirty = true;
    lightQueue.push({position, lightLevel});

    while (!lightQueue.empty()) {
        lightPropogationNode n = lightQueue.front();
        lightQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; i++) {
            newPos = n.position + adjacent[i];
            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                if (chunk == this) {
                    if (getBlock(newPos) == 0 && getLightLevel(newPos, channel) < n.lightLevel - 1) {
                        data[flattenVector(getRelativePosition(newPos))].lightLevel[channel] =
                                n.lightLevel - (LightLevel) 1;
                        meshDirty = true;
                        lightQueue.push({newPos, (LightLevel) (n.lightLevel - 1)});
                    }
                } else if (chunk->getBlock(newPos) == 0 && chunk->getLightLevel(newPos, channel) < n.lightLevel - 1) {
                    chunk->addLight(newPos, (LightLevel) (n.lightLevel - 1), channel);
                }
            }
        }
    }

}

void Chunk::addSunlight(fvec3 position, LightLevel lightLevel) {
    std::queue<lightPropogationNode> lightQueue;

    data[flattenVector(getRelativePosition(position))].sunlightLevel = lightLevel;
    meshDirty = true;
    lightQueue.push({position, lightLevel});

    while (!lightQueue.empty()) {
        lightPropogationNode n = lightQueue.front();
        lightQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; i++) {
            newPos = n.position + adjacent[i];
            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                if (chunk == this) {
                    if (i == 4) {
                        if (getBlock(newPos) == 0 && getSunlightLevel(newPos) < n.lightLevel &&
                            getSunlightLevel(newPos) > -1) {
                            data[flattenVector(getRelativePosition(newPos))].sunlightLevel = n.lightLevel;
                            meshDirty = true;
                            lightQueue.push({newPos, n.lightLevel});
                        }
                    } else if (getBlock(newPos) == 0 && getSunlightLevel(newPos) < n.lightLevel - 1 &&
                               getSunlightLevel(newPos) > -1) {
                        data[flattenVector(getRelativePosition(newPos))].sunlightLevel = n.lightLevel - (LightLevel) 1;
                        meshDirty = true;
                        lightQueue.push({newPos, (LightLevel) (n.lightLevel - 1)});
                    }
                } else if (chunk->getBlock(newPos) == 0 && chunk->getSunlightLevel(newPos) < n.lightLevel - 1 &&
                           chunk->getSunlightLevel(newPos) > -1) {
                    chunk->addSunlight(newPos, (LightLevel) (n.lightLevel - 1));
                }
            }
        }
    }
}

void Chunk::removeLight(fvec3 position, int channel) {
    nullifyLight(position, channel);
    fillLight(position, 0, channel);
}
void Chunk::removeSunlight(fvec3 position){
    nullifySunlight(position);
    fillSunlight(position, 0);
}


void Chunk::nullifyLight(fvec3 position, int channel) {
    std::queue<lightPropogationNode> removalQueue;
    std::queue<lightPropogationNode> otherRemovalQueue;

    removalQueue.push({position, getLightLevel(position, channel)});
    setLightLevel(position, -1, channel);

    while (!removalQueue.empty()) {
        lightPropogationNode n = removalQueue.front();
        removalQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; i++) {
            newPos = n.position + adjacent[i];
            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                LightLevel newLight = chunk->getLightLevel(newPos, channel);
                if (chunk == this) {
                    if (newLight < n.lightLevel) {
                        if (getBlock(newPos) == 0 && newLight > 0) {
                            removalQueue.push({newPos, newLight});
                            setLightLevel(newPos, -1, channel);
                            meshDirty = true;
                        }
                    }
                } else if (newLight < n.lightLevel) {
                    if (chunk->getBlock(newPos) == 0 && newLight > 0) {
                        otherRemovalQueue.push({newPos, newLight});
                    }
                }
            }
        }
    }

    while (!otherRemovalQueue.empty()) {
        lightPropogationNode n = otherRemovalQueue.front();
        otherRemovalQueue.pop();

        Chunk *chunk = getChunk(n.position);
        if (chunk) {
            chunk->nullifyLight(n.position, channel);
        }
    }
}

void Chunk::fillLight(fvec3 position, LightLevel lightLevel, int channel) {
    std::queue<lightPropogationNode> fillQueue;

    setLightLevel(position, 0, channel);
    meshDirty = true;
    fillQueue.push({position, lightLevel});
    while (!fillQueue.empty()) {
        lightPropogationNode n = fillQueue.front();
        fillQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; ++i) {
            newPos = n.position + adjacent[i];

            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                if (chunk == this) {
                    if (getBlock(newPos) == 0 && getLightLevel(newPos, channel) == -1) {
                        setLightLevel(newPos, 0, channel);
                        meshDirty = true;
                        fillQueue.push({newPos, (LightLevel) (n.lightLevel - 1)});
                    } else {
                        addLight(newPos, getLightLevel(newPos, channel), channel);
                    }
                } else {
                    if (chunk->getBlock(newPos) == 0 && chunk->getLightLevel(newPos, channel) == -1) {
                        chunk->fillLight(newPos, (LightLevel) (n.lightLevel - 1), channel);
                    }
                }
            }
        }
    }
}

void Chunk::nullifySunlight(fvec3 position) {
    std::queue<lightPropogationNode> removalQueue;
    std::queue<lightPropogationNode> otherRemovalQueue;

    removalQueue.push({position, getSunlightLevel(position)});
    setSunlightLevel(position, -1);

    while (!removalQueue.empty()) {
        lightPropogationNode n = removalQueue.front();
        removalQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; i++) {
            newPos = n.position + adjacent[i];
            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                LightLevel newLight = chunk->getSunlightLevel(newPos);
                if (chunk == this) {
                    if ((i == 4 && newLight <= n.lightLevel) || newLight < n.lightLevel) {
                        if (getBlock(newPos) == 0 && newLight > 0) {
                            removalQueue.push({newPos, newLight});
                            setSunlightLevel(newPos, -1);
                            meshDirty = true;
                        }
                    }
                } else if (newLight < n.lightLevel) {
                    if (chunk->getBlock(newPos) == 0 && newLight > 0) {
                        otherRemovalQueue.push({newPos, newLight});
                    }
                }
            }
        }
    }

    while (!otherRemovalQueue.empty()) {
        lightPropogationNode n = otherRemovalQueue.front();
        otherRemovalQueue.pop();

        Chunk *chunk = getChunk(n.position);
        if (chunk) {
            chunk->nullifySunlight(n.position);
        }
    }
}

void Chunk::fillSunlight(fvec3 position, LightLevel lightLevel) {
    std::queue<lightPropogationNode> fillQueue;

    setSunlightLevel(position, 0);
    fillQueue.push({position, lightLevel});
    while (!fillQueue.empty()) {
        lightPropogationNode n = fillQueue.front();
        fillQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; ++i) {
            newPos = n.position + adjacent[i];

            Chunk *chunk = getChunk(newPos);
            if (chunk) {
                if (chunk == this) {
                    if (getBlock(newPos) == 0 && getSunlightLevel(newPos) == -1) {
                        setSunlightLevel(newPos, 0);
                        meshDirty = true;
                        fillQueue.push({newPos, (LightLevel) (n.lightLevel - 1)});
                    } else {
                        addSunlight(newPos, getSunlightLevel(newPos));
                    }
                } else {
                    if (chunk->getBlock(newPos) == 0 && chunk->getSunlightLevel(newPos) == -1) {
                        chunk->fillSunlight(newPos, (LightLevel) (n.lightLevel - 1));
                    } else {
                        chunk->addSunlight(newPos, getSunlightLevel(newPos));
                    }
                }
            }
        }
    }
}

void Chunk::updateLight(fvec3 position, int channel) {
    if (getBlock(position) != 0) {
        return;
    }
    LightLevel max_adjacent = 0;
    int max_adjacent_i = 0;
    for (int i = 0; i < 6; ++i) {
        Chunk *chunk = getChunk(position);
        if (chunk) {
            LightLevel l = chunk->getLightLevel(position + adjacent[i], channel);
            if (l > max_adjacent) {
                max_adjacent = l;
                max_adjacent_i = i;
            }
        }
    }
    if (max_adjacent > getLightLevel(position, channel)) {
        addLight(position + adjacent[max_adjacent_i], max_adjacent, channel);
    } else {
        addLight(position, getLightLevel(position, channel), channel);
    }
}

void Chunk::updateSunlight(fvec3 position) {
    if (getBlock(position) != 0) {
        return;
    }
    if (position == fvec3{15, 0, 40})
        position = position;
    LightLevel max_adjacent = 0;
    int max_adjacent_i = -1;
    for (int i = 0; i < 6; ++i) {
        Chunk *chunk = getChunk(position + adjacent[i]);
        if (chunk) {
            LightLevel l = chunk->getSunlightLevel(position + adjacent[i]);
            if (chunk->getBlock(position + adjacent[i]) == 0 && l > max_adjacent) {
                max_adjacent = l;
                max_adjacent_i = i;
            }
        }
    }
    if (max_adjacent_i > -1) {
        Chunk *chunk = getChunk(position + adjacent[max_adjacent_i]);
        if (max_adjacent > getSunlightLevel(position)) {
            chunk->addSunlight(position + adjacent[max_adjacent_i], max_adjacent);
        } else {
            addSunlight(position, getSunlightLevel(position));
        }
    }
}

LightLevel Chunk::getLightLevel(fvec3 worldPosition, int channel) {
    fvec3 rel = getRelativePosition(worldPosition);
    if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y) {
        return -1;
    } else if (rel.x < 0 || rel.z < 0 || rel.z >= CHUNK_SIZE_Z || rel.x >= CHUNK_SIZE_X) {
        Chunk *chunk = getChunk(worldPosition);
        if (chunk) {
            return chunk->getLightLevel(worldPosition, channel);
        } else
            return 0;
    } else
        return data[flattenVector(floor(rel))].lightLevel[channel];
}

void Chunk::setLightLevel(fvec3 worldPosition, LightLevel lightLevel, int channel) {
    fvec3 position = getRelativePosition(worldPosition);
    if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X ||
        position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
        return;
    data[flattenVector(floor(position))].lightLevel[channel] = lightLevel;
    meshDirty = true;
}

LightLevel Chunk::getSunlightLevel(fvec3 worldPosition) {
    Chunk *chunk = getChunk(worldPosition);
    if (chunk) {
        if (chunk == this) {
            fvec3 rel = getRelativePosition(worldPosition);
            return data[flattenVector(floor(rel))].sunlightLevel;
        } else {
            return chunk->getSunlightLevel(worldPosition);
        }
    }
    return 0;
}

void Chunk::setSunlightLevel(fvec3 worldPosition, LightLevel lightLevel) {
    fvec3 position = getRelativePosition(worldPosition);
    if (position.y >= CHUNK_SIZE_Y || position.y < 0)
        return;
    if (position.x < 0 || position.z < 0 || position.x >= CHUNK_SIZE_X || position.z >= CHUNK_SIZE_Z) {
        world->setSunlightLevel(worldPosition, lightLevel);
        return;
    }
    data[flattenVector(floor(position))].sunlightLevel = lightLevel;
    meshDirty = true;
}

void Chunk::updateBuffer() {
    graphics->updateBuffer(bufferResourceId, mesh, meshLength);
    delete[] mesh;
}

void Chunk::render() {
    if (state >= ChunkState::Loaded) {
        graphics->renderBuffer(bufferResourceId, meshLength / 11);
    }
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
                if (getBlock(getRelativePosition({i, j, k})) > 0)
                    return true;
            }
        }
    }
    return false;
}

Chunk *Chunk::getChunk(fvec3 position) {
    fvec3 relPosition = getRelativePosition(position);
    if (relPosition.y < 0 || relPosition.y >= CHUNK_SIZE_Y) {
        return nullptr;
    }

    Chunk *chunk;
    if (relPosition.x < 0) {
        chunk = neighbors[3];
    } else if (relPosition.z < 0) {
        chunk = neighbors[0];
    } else if (relPosition.z >= CHUNK_SIZE_Z) {
        chunk = neighbors[1];
    } else if (relPosition.x >= CHUNK_SIZE_X) {
        chunk = neighbors[2];
    } else {
        return this;
    }

    if (chunk && chunk->isLoaded()) {
        return chunk;
    }

    return nullptr;
}

Chunk *Chunk::getChunkRelative(fvec3 relPosition) {
    if (relPosition.y < 0 || relPosition.y >= CHUNK_SIZE_Y) {
        return nullptr;
    }
    if (relPosition.x < 0) {
        return neighbors[3];
    } else if (relPosition.z < 0) {
        return neighbors[0];
    } else if (relPosition.z >= CHUNK_SIZE_Z) {
        return neighbors[1];
    } else if (relPosition.x >= CHUNK_SIZE_X) {
        return neighbors[2];
    } else {
        return this;
    }

}

int Chunk::getBlock(fvec3 worldPosition) {
    fvec3 rel = getRelativePosition(worldPosition);
    if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y)
        return -1;
    else if (rel.x < 0 || rel.z < 0 || rel.z >= CHUNK_SIZE_Z || rel.x >= CHUNK_SIZE_X) {
        return world->getBlock(worldPosition);
    } else
        return data[flattenVector(floor(rel))].blockId;
}

int Chunk::getBlockRelative(fvec3 worldPosition) {
    fvec3 rel = getRelativePosition(worldPosition);
    if (rel.y < 0 || rel.y >= CHUNK_SIZE_Y)
        return -1;
    else {
        Chunk *chunk = getChunkRelative(rel);
        if (chunk) {
            return chunk->getBlock(worldPosition);
        }
    }
    return -1;
}

void Chunk::setBlock(fvec3 worldPosition, unsigned int blockId) {
    fvec3 position = getRelativePosition(worldPosition);
    if (position.x < 0 || position.z < 0 || position.y < 0 || position.x >= CHUNK_SIZE_X ||
        position.z >= CHUNK_SIZE_Z || position.y >= CHUNK_SIZE_Y)
        return;
    data[flattenVector(floor(position))].blockId = blockId;
    meshDirty = true;
}

void Chunk::addSunlightLocal(fvec3 position, LightLevel lightLevel) {
    std::queue<lightPropogationNode> lightQueue;

    data[flattenVector(getRelativePosition(position))].sunlightLevel = lightLevel;
    lightQueue.push({position, lightLevel});

    while (!lightQueue.empty()) {
        lightPropogationNode n = lightQueue.front();
        lightQueue.pop();

        fvec3 newPos;
        for (int i = 0; i < 6; i++) {
            newPos = n.position + adjacent[i];
            Chunk *chunk = getChunk(newPos);
            if (chunk == this) {
                if (i == 4) {
                    if (getBlock(newPos) == 0 && getSunlightLevel(newPos) < n.lightLevel) {
                        data[flattenVector(getRelativePosition(newPos))].sunlightLevel = n.lightLevel;
                        meshDirty = true;
                        lightQueue.push({newPos, n.lightLevel});
                    }
                } else if (getBlock(newPos) == 0 && getSunlightLevel(newPos) < n.lightLevel - 1) {
                    data[flattenVector(getRelativePosition(newPos))].sunlightLevel = n.lightLevel - (LightLevel) 1;
                    meshDirty = true;
                    lightQueue.push({newPos, (LightLevel) (n.lightLevel - 1)});
                }
            }
        }
    }
}

bool Chunk::isLoaded() {
    return state >= ChunkState::Loaded;
}

int Chunk::flattenVector(fvec3 coords) {
    coords = floor(coords);
    return (int) floor(coords.y + CHUNK_SIZE.y * (coords.x + CHUNK_SIZE.x * coords.z));
}

fvec3 Chunk::getRelativePosition(fvec3 worldPosition) {
    return worldPosition - (fvec3(chunkCoords.x, chunkCoords.z, 0) * CHUNK_SIZE);
}

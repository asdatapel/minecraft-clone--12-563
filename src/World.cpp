#include "World.h"

World::World(GraphicsManager *graphicsManager, int shaderId) {
    graphics = graphicsManager;
    shader = shaderId;

    std::thread t1(&World::meshTask, this);
    t1.detach();
    std::thread t2(&World::loadTask, this);
    t2.detach();
}

World::~World() {

}

void World::genChunks(Perlin *p) {
    blocks[1].id = 1;
    blocks[1].textures[0].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].textures[1].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].textures[2].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].textures[3].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].textures[4].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].textures[5].texId = graphics->getArrayTexture("textures/stone.png");
    blocks[1].lightOutput[0] = 0;
    blocks[1].lightOutput[1] = 15;
    blocks[1].lightOutput[2] = 0;
    blocks[2].id = 2;
    blocks[2].textures[0].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].textures[1].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].textures[2].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].textures[3].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].textures[4].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].textures[5].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[2].lightOutput[0] = 0;
    blocks[2].lightOutput[1] = 0;
    blocks[2].lightOutput[2] = 0;
    blocks[3].id = 3;
    blocks[3].textures[0].texId = graphics->getArrayTexture("textures/grass_side.png");
    blocks[3].textures[1].texId = graphics->getArrayTexture("textures/grass_side.png");
    blocks[3].textures[2].texId = graphics->getArrayTexture("textures/grass_side.png");
    blocks[3].textures[3].texId = graphics->getArrayTexture("textures/grass_side.png");
    blocks[3].textures[4].texId = graphics->getArrayTexture("textures/dirt.png");
    blocks[3].textures[5].texId = graphics->getArrayTexture("textures/grass.png");
    blocks[3].lightOutput[0] = 0;
    blocks[3].lightOutput[1] = 0;
    blocks[3].lightOutput[2] = 0;
    blocks[4].id = 4;
    blocks[4].lightOutput[0] = 15;
    blocks[4].lightOutput[1] = 0;
    blocks[4].lightOutput[2] = 0;
    blocks[4].textures[0].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[4].textures[1].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[4].textures[2].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[4].textures[3].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[4].textures[4].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[4].textures[5].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
    blocks[5].id = 5;
    blocks[5].lightOutput[0] = 15;
    blocks[5].lightOutput[1] = 0;
    blocks[5].lightOutput[2] = 0;
    blocks[5].textures[0].texId = graphics->getArrayTexture("textures/red.png");
    blocks[5].textures[1].texId = graphics->getArrayTexture("textures/red.png");
    blocks[5].textures[2].texId = graphics->getArrayTexture("textures/red.png");
    blocks[5].textures[3].texId = graphics->getArrayTexture("textures/red.png");
    blocks[5].textures[4].texId = graphics->getArrayTexture("textures/red.png");
    blocks[5].textures[5].texId = graphics->getArrayTexture("textures/red.png");
    blocks[6].id = 6;
    blocks[6].lightOutput[0] = 0;
    blocks[6].lightOutput[1] = 15;
    blocks[6].lightOutput[2] = 0;
    blocks[6].textures[0].texId = graphics->getArrayTexture("textures/green.png");
    blocks[6].textures[1].texId = graphics->getArrayTexture("textures/green.png");
    blocks[6].textures[2].texId = graphics->getArrayTexture("textures/green.png");
    blocks[6].textures[3].texId = graphics->getArrayTexture("textures/green.png");
    blocks[6].textures[4].texId = graphics->getArrayTexture("textures/green.png");
    blocks[6].textures[5].texId = graphics->getArrayTexture("textures/green.png");
    blocks[7].id = 7;
    blocks[7].lightOutput[0] = 0;
    blocks[7].lightOutput[1] = 0;
    blocks[7].lightOutput[2] = 15;
    blocks[7].textures[0].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[7].textures[1].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[7].textures[2].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[7].textures[3].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[7].textures[4].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[7].textures[5].texId = graphics->getArrayTexture("textures/blue.png");
    blocks[8].id = 8;
    blocks[8].lightOutput[0] = 0;
    blocks[8].lightOutput[1] = 0;
    blocks[8].lightOutput[2] = 0;
    blocks[8].textures[0].texId = graphics->getArrayTexture("textures/white.png");
    blocks[8].textures[1].texId = graphics->getArrayTexture("textures/white.png");
    blocks[8].textures[2].texId = graphics->getArrayTexture("textures/white.png");
    blocks[8].textures[3].texId = graphics->getArrayTexture("textures/white.png");
    blocks[8].textures[4].texId = graphics->getArrayTexture("textures/white.png");
    blocks[8].textures[5].texId = graphics->getArrayTexture("textures/white.png");
    blocks[9].id = 9;
    blocks[9].lightOutput[0] = 0;
    blocks[9].lightOutput[1] = 0;
    blocks[9].lightOutput[2] = 0;
    blocks[9].textures[0].texId = graphics->getArrayTexture("textures/log_birch.png");
    blocks[9].textures[1].texId = graphics->getArrayTexture("textures/log_birch.png");
    blocks[9].textures[2].texId = graphics->getArrayTexture("textures/log_birch.png");
    blocks[9].textures[3].texId = graphics->getArrayTexture("textures/log_birch.png");
    blocks[9].textures[4].texId = graphics->getArrayTexture("textures/log_birch_top.png");
    blocks[9].textures[5].texId = graphics->getArrayTexture("textures/log_birch_top.png");

    perlin = p;

}

bool World::checkCollision(AABB box) {
    fvec3 pos = box.getPosition();
    fvec3 bound = pos + box.getSize();

    ivec2 blockCoords = getChunkCoords(pos);
    Chunk *chunk = getChunk(blockCoords);
    if (!chunk || chunk->checkCollision(box))
        return true;

    blockCoords = getChunkCoords({(int) pos.x, (int) bound.z});
    chunk = getChunk(blockCoords);
    if (!chunk || chunk->checkCollision(box))
        return true;

    blockCoords = getChunkCoords({(int) bound.x, (int) pos.z});
    chunk = getChunk(blockCoords);
    if (!chunk || chunk->checkCollision(box))
        return true;

    blockCoords = getChunkCoords(bound);
    chunk = getChunk(blockCoords);
    if (!chunk || chunk->checkCollision(box))
        return true;

    return false;
}

bool World::checkCollision(BoundingBox box) {
    AABB b = box.getAABB();

    fvec3 pos = b.getPosition();
    fvec3 bounds = pos + b.getSize();

    for (int i = floor(pos.x); i <= bounds.x; i++) {
        for (int j = floor(pos.z); j <= bounds.z; j++) {
            for (int k = floor(pos.y); k <= bounds.y; k++) {
                if (box.checkCollision({i, j, k})) {
                    if (getBlock({i, j, k}) > 0)
                        return true;
                }
            }
        }
    }

    return false;
}

RayCollision World::raycast(fvec3 position, fvec3 direction, float distance) {
    RayCollision ray;
    ray.distance = 0;
    ray.side = 0;
    ray.isCollision = false;

    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);
    if (!chunk)
        return ray;

    if (!chunk->raycast(position, direction, distance, &ray) && ray.distance < distance) {
        chunkCoords += adjacent_chunk[ray.side];
        chunk = getChunk(chunkCoords);
        if (!chunk)
            return ray;
        if (!chunk->raycast(position, direction, distance, &ray) && ray.distance < distance) {
            chunkCoords += adjacent_chunk[ray.side];
            chunk = getChunk(chunkCoords);
            if (!chunk)
                return ray;
            chunk->raycast(position, direction, distance, &ray);
        }
    }
    return ray;
}

int World::getBlock(fvec3 position) {

    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);
    if (!chunk)
        return -1;

    if (!chunk->isLoaded())
        return -1;
    return chunk->getBlock(position);
}

void World::removeBlock(fvec3 position) {
    if (getBlock(position) != 0) {
        setBlock(position, 0);
        removeLight(position, 0);
        removeLight(position, 1);
        removeLight(position, 2);
        removeSunlight(position);
    }
}

void World::addBlock(fvec3 position, int blockId) {
    position = floor(position);
    if (getBlock(position) == 0) {
        setBlock(position, blockId);
        if (blocks[blockId].lightOutput[0] > 0) {//check for all channels
            addLight(position, blocks[blockId].lightOutput[0], 0);
        } else {
            removeLight(position, 0);
        }
        if (blocks[blockId].lightOutput[1] > 1) {//check for all channels
            addLight(position, blocks[blockId].lightOutput[1], 1);
        } else {
            removeLight(position, 1);
        }
        if (blocks[blockId].lightOutput[2] > 2) {//check for all channels
            addLight(position, blocks[blockId].lightOutput[2], 2);
        } else {
            removeLight(position, 2);
        }
        removeSunlight(position);
    }
}

void World::setLightLevel(fvec3 position, unsigned short lightLevel) {
    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);

    if (!chunk)
        return;

    chunk->setLightLevel(position, lightLevel, 0);
}

LightLevel World::getLightLevel(fvec3 position) {
    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);

    if (!chunk)
        return -1;

    return chunk->getLightLevel(position, 0);
}

void World::setSunlightLevel(fvec3 position, unsigned short lightLevel) {
    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);

    if (!chunk)
        return;

    chunk->setSunlightLevel(position, lightLevel);
}

LightLevel World::getSunlightLevel(fvec3 position) {
    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);

    if (!chunk)
        return 0;

    if (!chunk->isLoaded())
        return 0;

    return chunk->getSunlightLevel(position);
}

void World::setBlock(fvec3 position, int blockId) {
    ivec2 chunkCoords = getChunkCoords(position);
    Chunk *chunk = getChunk(chunkCoords);

    if (!chunk)
        return;

    chunk->setBlock(position, blockId);

    int xBorder = (int) floor(position.x) % CHUNK_SIZE_X;
    int zBorder = (int) floor(position.z) % CHUNK_SIZE_Z;

    if (xBorder == 0) {
        chunkCoords += adjacent_chunk[3];
        chunk = getChunk(chunkCoords);
        if (chunk)
            chunk->meshDirty = false;
    } else if (xBorder == (CHUNK_SIZE_X - 1)) {

        chunkCoords += adjacent_chunk[2];
        chunk = getChunk(chunkCoords);
        if (chunk)
            chunk->meshDirty = false;
    }
    if (zBorder == 0) {
        chunkCoords += adjacent_chunk[0];
        chunk = getChunk(chunkCoords);
        if (chunk)
            chunk->meshDirty = false;
    } else if (zBorder == (CHUNK_SIZE_Z - 1)) {
        chunkCoords += adjacent_chunk[1];
        chunk = getChunk(chunkCoords);
        if (chunk)
            chunk->meshDirty = false;
    }
}

#define VISIBLE_CHUNKS 20

void World::update(fvec3 playerPos, float elapsedTime) {
    std::unique_lock<std::mutex> lock(m);

    ivec2 centerChunk = getChunkCoords(playerPos);
    chunkLoadCenter = centerChunk;
    for (auto chunk = chunks.begin(); chunk != chunks.end();) {
        ivec2 vec = chunk->first;
        if (!isChunkInLoadRange(vec, centerChunk)) {
            if (chunk->second->state == ChunkState::InLoadingQueue) {
                loadJobs.remove(chunk->second);
            } else if (chunk->second->state == ChunkState::InMeshingQueue) {
                meshJobs.remove(chunk->second);
            }
            if (chunk->second->state != ChunkState::CurrentlyLoading &&
                chunk->second->state != ChunkState::CurrentlyMeshing) {
                for (int k = 0; k < 4; ++k) {
                    Chunk *neighbor = chunk->second->neighbors[k];
                    if (neighbor) {
                        neighbor->neighbors[adjacent_opposite[k]] = nullptr;
                    }
                }
                delete chunk->second;
                chunk = chunks.erase(chunk);
                continue;
            }
        }
        ++chunk;
    }

    for (int i = centerChunk.x - VISIBLE_CHUNKS; i != centerChunk.x + VISIBLE_CHUNKS + 1; i++) {
        for (int j = centerChunk.z - VISIBLE_CHUNKS; j != centerChunk.z + VISIBLE_CHUNKS + 1; j++) {
            if (isChunkInLoadRange({i, j}, centerChunk)) {
                Chunk *chunk = getChunk({i, j});
                if (!chunk) {
                    chunk = new Chunk(this, perlin, ivec2(i, j), blocks, graphics, shader);
                    chunks.insert({{i, j}, chunk});
                    for (int k = 0; k < 4; ++k) {
                        Chunk *neighbor = getChunk(ivec2{i, j} + adjacent_chunk[k]);
                        chunk->neighbors[k] = neighbor;
                        if (neighbor) {
                            neighbor->neighbors[adjacent_opposite[k]] = chunk;
                        }
                    }
                    chunk->state = ChunkState::InLoadingQueue;
                    loadJobs.push_front(chunk);

                    loadWaiter.notify_one();
                } else if (chunk->meshDirty &&
                           (chunk->state == ChunkState::Loaded || chunk->state == ChunkState::Meshed) &&
                           isChunkNeighborsLoaded({i, j})) {
                    chunk->state = ChunkState::InMeshingQueue;
                    meshJobs.push_front(chunk);
                    meshWaiter.notify_one();
                }

            }
        }
    }

    while (!finishedLoadJobs.empty()) {
        Chunk *job = finishedLoadJobs.front();
        finishedLoadJobs.pop();

        job->state = ChunkState::Loaded;

        for (int k = 0; k < CHUNK_SIZE_Y; ++k) {
            for (int i = job->chunkCoords.x * CHUNK_SIZE_X; i < job->chunkCoords.x * CHUNK_SIZE_X + CHUNK_SIZE_X; ++i) {
                if (job->neighbors[0] && job->neighbors[0]->isLoaded()) {
                    job->neighbors[0]->updateSunlight({i, job->chunkCoords.z * CHUNK_SIZE_Z - 1, k});
                }
                if (job->neighbors[1] && job->neighbors[1]->isLoaded()) {
                    job->neighbors[1]->updateSunlight({i, job->chunkCoords.z * CHUNK_SIZE_Z + CHUNK_SIZE_Z, k});
                }
            }
            for (int j = job->chunkCoords.z * CHUNK_SIZE_Z; j < job->chunkCoords.z * CHUNK_SIZE_Z + CHUNK_SIZE_Z; ++j) {
                if (job->neighbors[3] && job->neighbors[3]->isLoaded()) {
                    job->neighbors[3]->updateSunlight({job->chunkCoords.x * CHUNK_SIZE_X - 1, j, k});
                }
                if (job->neighbors[2] && job->neighbors[2]->isLoaded()) {
                    job->neighbors[2]->updateSunlight({job->chunkCoords.x * CHUNK_SIZE_X + CHUNK_SIZE_X, j, k});
                }
            }
        }

        for (int i = 0; i < 4; ++i) {
            if (job->neighbors[i]) {
                job->neighbors[i]->meshDirty = true;
            }
        }

        job->meshDirty = true;
    }

    while (!finishedMeshJobs.empty()) {
        Chunk *job = finishedMeshJobs.front();
        finishedMeshJobs.pop();

        job->state = ChunkState::Meshed;
        job->meshDirty = false;

        job->meshLength = job->tempMeshLength;

        job->updateBuffer();
    }

}

void World::renderChunks() {
    for (auto const &chunk : chunks) {
        chunk.second->render();
    }
}

Chunk *World::getChunk(ivec2 coords) {
    auto element = chunks.find(coords);
    if (element != chunks.end())
        return chunks[coords];
    return NULL;
}

ivec2 World::getChunkCoords(fvec3 worldPosition) {
    worldPosition = floor(worldPosition);
    return {worldPosition.x / CHUNK_SIZE_X, worldPosition.z / CHUNK_SIZE_Z};
}

ivec2 World::getChunkCoords(ivec2 worldPosition) {
    return {(float) worldPosition.x / (float) CHUNK_SIZE_X, (float) worldPosition.z / (float) CHUNK_SIZE_Z};
}

bool World::isChunkInLoadRange(ivec2 chunkCoords, ivec2 playerCoords) {
    //return pow(chunkCoords.x - playerCoords.x, 2) + pow(chunkCoords.z - playerCoords.z, 2) < pow(VISIBLE_CHUNKS, 2);
    return pow(chunkCoords.x, 2) + pow(chunkCoords.z, 2) < pow(VISIBLE_CHUNKS, 2);
}

bool World::isChunkInMeshRange(ivec2 chunkCoords, ivec2 playerCoords) {
    return pow(chunkCoords.x - playerCoords.x, 2) + pow(chunkCoords.z - playerCoords.z, 2) < pow(VISIBLE_CHUNKS - 1, 2);
}

bool World::isChunkNeighborsLoaded(ivec2 chunkCoords) {
    Chunk *chunk;
    chunkCoords += adjacent_chunk[0];
    chunk = getChunk(chunkCoords);
    if (!chunk || !chunk->isLoaded())
        return false;
    chunkCoords += adjacent_chunk[1];
    chunk = getChunk(chunkCoords);
    if (!chunk || !chunk->isLoaded())
        return false;
    chunkCoords += adjacent_chunk[2];
    chunk = getChunk(chunkCoords);
    if (!chunk || !chunk->isLoaded())
        return false;
    chunkCoords += adjacent_chunk[3];
    chunk = getChunk(chunkCoords);
    if (!chunk || !chunk->isLoaded())
        return false;
    return true;
}

void World::addLight(fvec3 position, LightLevel lightLevel, int channel) {
    Chunk *chunk = getChunk(getChunkCoords(position));
    if (chunk) {
        chunk->addLight(position, lightLevel, channel);
    }
}

void World::removeLight(fvec3 position, int channel) {
    Chunk *chunk = getChunk(getChunkCoords(position));
    if (chunk) {
        chunk->removeLight(position, channel);
    }
}

void World::removeSunlight(fvec3 position) {
    Chunk *chunk = getChunk(getChunkCoords(position));
    if (chunk) {
        chunk->removeSunlight(position);
    }
}

void World::addCompleteJobToQueue(std::queue<Chunk *> *q, Chunk *job) {
    std::unique_lock<std::mutex> lock(m);
    q->push(job);
}

Chunk *World::getNextJob(std::list<Chunk *> *jobs) {
    std::list<Chunk *>::iterator max_i;
    int min = 3000;
    for (std::list<Chunk *>::iterator i = jobs->begin(); i != jobs->end(); ++i) {
        int d = std::abs((*i)->chunkCoords.x - chunkLoadCenter.x) + std::abs((*i)->chunkCoords.z - chunkLoadCenter.z);
        if (d < min) {
            min = d;
            max_i = i;
        }
    }
    Chunk *job = *max_i;
    jobs->erase(max_i);
    return job;
}

void World::meshTask() {
    while (true) {
        Chunk *job = getNextMeshJob();
        job->genMesh();
        addCompleteJobToQueue(&finishedMeshJobs, job);

    }
}

Chunk *World::getNextMeshJob() {
    std::unique_lock<std::mutex> lock(m);

    while (true) {
        if (!meshJobs.empty()) {
            Chunk *job = getNextJob(&meshJobs);
            job->state = ChunkState::CurrentlyMeshing;
            return job;
        }

        meshWaiter.wait(lock);
    }
}

void World::loadTask() {
    while (true) {
        Chunk *job = getNextLoadJob();
        genChunk(job);
        addCompleteJobToQueue(&finishedLoadJobs, job);

    }
}

Chunk *World::getNextLoadJob() {
    std::unique_lock<std::mutex> lock(m);

    while (true) {
        if (!loadJobs.empty()) {
            Chunk *job = getNextJob(&loadJobs);
            job->state = ChunkState::CurrentlyLoading;
            return job;
        }

        loadWaiter.wait(lock);
    }
}

void World::physTask() {
    while (true) {
        Box *box = getNextPhysObj();
        //box->loadData();
    }
}

Box *World::getNextPhysObj() {
    std::unique_lock<std::mutex> lock(physMutex);

    while (true) {
        if (!boxes.empty()) {
            Box *box = boxes.front();
            boxes.pop_front();
            return box;
        }

        loadWaiter.wait(lock);
    }
}

void World::createBoxes(int shaderId) {
    for (int i = 0; i < 10; ++i) {
        AABB a;
        a.setPosition({i * 1, i * 1, 100});
        a.setSize({1, 1, 2});

        Box *b = new Box(graphics, a, shaderId);
        boxes.push_back(b);
    }
}

void World::renderBoxes(glm::mat4 *PV) {
    for (auto &b : boxes) {
        b->render(PV);
    }
}

void World::createRandomBoxes(int shaderId) {
    for (int i = 0; i < 100; ++i) {
        AABB a;
        a.setPosition({std::rand() % 300 - 150, std::rand() % 100 - 50, 100 + std::rand() % 30});
        a.setSize({1, 1, 1 + std::rand() % 3});

        Box *b = new Box(graphics, a, shaderId);
        boxes.push_back(b);
    }
}

void World::genChunk(Chunk *chunk) {
    Block1 *data = chunk->data;
    ivec2 chunkCoords = chunk->chunkCoords;

    fvec3 coords;
    for (coords.x = 0; coords.x < CHUNK_SIZE_X; coords.x++) {
        for (coords.z = 0; coords.z < CHUNK_SIZE_Z; coords.z++) {

            int dirtHeight =
                    (int) (15 * octave_noise_3d(4, 1, 1, (coords.x + chunkCoords.x * CHUNK_SIZE_X) * 0.003f,
                                                (coords.z + chunkCoords.z * CHUNK_SIZE_Z) * 0.003f, 20)) +
                    (int) (50 + 30 * octave_noise_3d(4, 2, 0.5, (coords.x + chunkCoords.x * CHUNK_SIZE_X) * 0.0009f,
                                                     (coords.z + chunkCoords.z * CHUNK_SIZE_Z) * 0.0009f, 3645));
            for (coords.y = 0; coords.y < dirtHeight; ++coords.y) {
                data[flattenVector(coords)].blockId = 2;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;
            }

            coords.y = dirtHeight;
            data[flattenVector(coords)].blockId = 3;
            data[flattenVector(coords)].sunlightLevel = 0;
            data[flattenVector(coords)].lightLevel[0] = 0;
            data[flattenVector(coords)].lightLevel[1] = 0;
            data[flattenVector(coords)].lightLevel[2] = 0;

            for (coords.y = dirtHeight + 1; coords.y < CHUNK_SIZE_Y; ++coords.y) {
                data[flattenVector(coords)].blockId = 0;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;
            }

            for (coords.y = 0; coords.y < CHUNK_SIZE_Y; coords.y++) {
                fvec3 worldCoords = {chunkCoords.x * CHUNK_SIZE_X + coords.x,
                                     chunkCoords.z * CHUNK_SIZE_Z + coords.z, coords.y};
                float val1 = octave_noise_3d(1, 0.6, 1.2, worldCoords.x / 75.f, worldCoords.z / 75.f,
                                             worldCoords.y / 45.f) * (1.5f - worldCoords.y / 256.0f);
                float val2 = octave_noise_3d(2, 0.6, 1.2, worldCoords.x / 75.f, worldCoords.z / 75.f,
                                             worldCoords.y / 85.f) * (1.5f - worldCoords.y / 256.0f);

                if (val1 <= 0.1 && val1 >= -0.11 && val2 <= 0.1 && val2 >= -0.1) {

                    data[flattenVector(coords)].blockId = 0;
                    data[flattenVector(coords)].sunlightLevel = 0;
                    data[flattenVector(coords)].lightLevel[0] = 0;
                    data[flattenVector(coords)].lightLevel[1] = 0;
                    data[flattenVector(coords)].lightLevel[2] = 0;
                }
            }

            float noise = (octave_noise_3d(4, 1, 1, (coords.x + chunkCoords.x * CHUNK_SIZE_X) * 0.09f,
                                           (coords.z + chunkCoords.z * CHUNK_SIZE_Z) * 0.09f, 20) + 1) / 2;
            float rand = std::rand() / (float) RAND_MAX;
            if (rand >= 0.5 && rand <= noise) {
                coords.y = CHUNK_SIZE_Y - 1;
                while (coords.y >= 0 && data[flattenVector(coords)].blockId == 0) {
                    --coords.y;
                }
                for (int i = coords.y + 1; i < coords.y + 6 && i < CHUNK_SIZE_Y; ++i) {
                    data[flattenVector(fvec3(coords.x, coords.z, (float) i))].blockId = 9;
                }
            }

            /*for (coords.y = 0; coords.y < 40; ++coords.y){
                data[flattenVector(coords)].blockId = 1;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;

            }
            for (coords.y = 40; coords.y < 60; ++coords.y){
                data[flattenVector(coords)].blockId = 0;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;
            }
            for (coords.y = 60; coords.y < 80; ++coords.y){
                data[flattenVector(coords)].blockId = 1;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;
            }
            for (coords.y = 80; coords.y < 256; ++coords.y){
                data[flattenVector(coords)].blockId = 0;
                data[flattenVector(coords)].sunlightLevel = 0;
                data[flattenVector(coords)].lightLevel[0] = 0;
                data[flattenVector(coords)].lightLevel[1] = 0;
                data[flattenVector(coords)].lightLevel[2] = 0;
            }*/

        }
    }

    for (int i = chunkCoords.x * CHUNK_SIZE_X; i < chunkCoords.x * CHUNK_SIZE_X + CHUNK_SIZE_X; ++i) {
        for (int j = chunkCoords.z * CHUNK_SIZE_Z; j < chunkCoords.z * CHUNK_SIZE_Z + CHUNK_SIZE_Z; ++j) {
            chunk->addSunlightLocal({i, j, CHUNK_SIZE_Y - 1}, 10);
        }
    }

}

int World::flattenVector(fvec3 coords) {
    return (int) floor(coords.y + CHUNK_SIZE.y * (coords.x + CHUNK_SIZE.x * coords.z));
}

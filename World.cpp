#include "World.h"

World::World(GraphicsManager *graphicsManager, int shaderId)
{
	graphics = graphicsManager;
	shader = shaderId;

}

World::~World()
{

}

void World::genChunks(Perlin *p)
{
	blocks[1].id = 1;
	blocks[1].textures[0].texId = graphics->getArrayTexture("textures/stone.png"); 
	blocks[1].textures[1].texId = graphics->getArrayTexture("textures/stone.png");
	blocks[1].textures[2].texId = graphics->getArrayTexture("textures/stone.png");
	blocks[1].textures[3].texId = graphics->getArrayTexture("textures/stone.png");
	blocks[1].textures[4].texId = graphics->getArrayTexture("textures/stone.png");
	blocks[1].textures[5].texId = graphics->getArrayTexture("textures/stone.png");
	blocks[2].id = 2;
	blocks[2].textures[0].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[2].textures[1].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[2].textures[2].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[2].textures[3].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[2].textures[4].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[2].textures[5].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[3].id = 3;
	blocks[3].textures[0].texId = graphics->getArrayTexture("textures/grass_side.png");
	blocks[3].textures[1].texId = graphics->getArrayTexture("textures/grass_side.png");
	blocks[3].textures[2].texId = graphics->getArrayTexture("textures/grass_side.png");
	blocks[3].textures[3].texId = graphics->getArrayTexture("textures/grass_side.png");
	blocks[3].textures[4].texId = graphics->getArrayTexture("textures/dirt.png");
	blocks[3].textures[5].texId = graphics->getArrayTexture("textures/grass.png");
	blocks[4].id = 4;
	blocks[4].lightOutput = 25;
	blocks[4].textures[0].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
	blocks[4].textures[1].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
	blocks[4].textures[2].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
	blocks[4].textures[3].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
	blocks[4].textures[4].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");
	blocks[4].textures[5].texId = graphics->getArrayTexture("textures/redstone_lamp_on.png");

	perlin = p;

	std::thread t1(&World::meshTask, this);
	t1.detach();
	std::thread t2(&World::loadTask, this);
	t2.detach();

}

bool World::checkCollision(AABB box)
{
	fvec3 pos = box.getPosition();
	fvec3 bound = pos + box.getSize();

	ivec2 blockCoords = getChunkCoords(pos);
	Chunk* chunk = getChunk(blockCoords);
	if (!chunk || chunk->checkCollision(box))
		return true;

	blockCoords = getChunkCoords({ (int)pos.x, (int)bound.z });
	chunk = getChunk(blockCoords);
	if (!chunk || chunk->checkCollision(box))
		return true;

	blockCoords = getChunkCoords({ (int)bound.x, (int)pos.z });
	chunk = getChunk(blockCoords);
	if (!chunk || chunk->checkCollision(box))
		return true;

	blockCoords = getChunkCoords(bound);
	chunk = getChunk(blockCoords);
	if (!chunk || chunk->checkCollision(box))
		return true;

	return false;
}

bool World::checkCollision(BoundingBox box)
{
	AABB b = box.getAABB();

	fvec3 pos = b.getPosition();
	fvec3 bounds = pos + b.getSize();

	for (int i = floor(pos.x); i <= bounds.x ; i++) {
		for (int j = floor(pos.z); j <= bounds.z; j++) {
			for (int k = floor(pos.y); k <= bounds.y; k++) {
				if (box.checkCollision({ i,j,k })) {
					if (getBlock({ i,j,k }) > 0)
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
	Chunk* chunk = getChunk(chunkCoords);
	if (!chunk)
		return ray;

	if (!chunk->raycast(position, direction, distance, &ray) && ray.distance < distance) {
		chunkCoords += adjecent_chunk[ray.side];
		chunk = getChunk(chunkCoords);
		if (!chunk)
			return ray;
		if (!chunk->raycast(position, direction, distance, &ray) && ray.distance < distance) {
			chunkCoords += adjecent_chunk[ray.side];
			chunk = getChunk(chunkCoords);
			if (!chunk)
				return ray;
			chunk->raycast(position, direction, distance, &ray);
		}
	}
	return ray;
}

int World::getBlock(fvec3 position)
{

	ivec2 chunkCoords = getChunkCoords(position);
	Chunk* chunk = getChunk(chunkCoords);
	if (!chunk)
		return -1;

	if (!chunk->isLoaded())
		return -1;
	return chunk->getBlock(position);
}

void World::removeBlock(fvec3 position)
{
	if (getBlock(position) != 0) {
		setBlock(position, 0);
		removeLight(position);
	}
}

void World::addBlock(fvec3 position, int blockId)
{
	if (getBlock(position) == 0) {
		setBlock(position, blockId);
		if (blocks[blockId].lightOutput > 0)
			addLight(position, blocks[blockId].lightOutput);
		else
			removeLight(position);
	}
}

void World::setLightLevel(fvec3 position, unsigned short lightLevel)
{
	ivec2 chunkCoords = getChunkCoords(position);
	Chunk * chunk = getChunk(chunkCoords);

	if (!chunk)
		return;

	chunk->setLightLevel(position, lightLevel);
}

unsigned short World::getLightLevel(fvec3 position)
{
	ivec2 chunkCoords = getChunkCoords(position);
	Chunk * chunk = getChunk(chunkCoords);

	if (!chunk)
		return 8;

	return chunk->getLightLevel(position);
}

void World::setSunlightLevel(fvec3 position, unsigned short lightLevel)
{
	ivec2 chunkCoords = getChunkCoords(position);
	Chunk * chunk = getChunk(chunkCoords);

	if (!chunk)
		return;

	chunk->setSunlightLevel(position, lightLevel);
}

unsigned short World::getSunlightLevel(fvec3 position)
{
	ivec2 chunkCoords = getChunkCoords(position);
	Chunk * chunk = getChunk(chunkCoords);

	if (!chunk)
		return 0;

	if (!chunk->isLoaded())
		return 0;

	return chunk->getSunlightLevel(position);
}

void World::setBlock(fvec3 position, int blockId) {
	ivec2 chunkCoords = getChunkCoords(position);
	Chunk * chunk = getChunk(chunkCoords);

	if (!chunk)
		return;

	chunk->setBlock(position, blockId);

	int xBorder = (int)floor(position.x) % CHUNK_SIZE_X;
	int zBorder = (int)floor(position.z) % CHUNK_SIZE_Z;

	if (xBorder == 0) {
		chunkCoords += adjecent_chunk[3];
		chunk = getChunk(chunkCoords);
		if (chunk)
			chunk->meshDirty = false;
	}
	else if (xBorder == (CHUNK_SIZE_X - 1)) {

		chunkCoords += adjecent_chunk[2];
		chunk = getChunk(chunkCoords);
		if (chunk)
			chunk->meshDirty = false;
	}
	if (zBorder == 0) {
		chunkCoords += adjecent_chunk[0];
		chunk = getChunk(chunkCoords);
		if (chunk)
			chunk->meshDirty = false;
	}
	else if (zBorder == (CHUNK_SIZE_Z - 1)) {
		chunkCoords += adjecent_chunk[1];
		chunk = getChunk(chunkCoords);
		if (chunk)
			chunk->meshDirty = false;
	}
}

#define VISIBLE_CHUNKS 5
void World::update(fvec3 playerPos, float elapsedTime)
{

    std::unique_lock<std::mutex> lock(m);

    ivec2 centerChunk = getChunkCoords(playerPos);
    for (auto chunk = chunks.begin(); chunk != chunks.end();) {
        ivec2 vec = chunk->first;
        if (!isChunkInLoadRange(vec, centerChunk)) {
            if (chunk->second->state == ChunkState::InLoadingQueue){
                loadJobs.remove(chunk->second);
            }else if(chunk->second->state == ChunkState::InMeshingQueue){
                meshJobs.remove(chunk->second);
            }
            if (chunk->second->state != ChunkState::CurrentlyLoading && chunk->second->state != ChunkState::CurrentlyMeshing) {
                delete chunk->second;
                chunk = chunks.erase(chunk);
                continue;
            }
        }
        ++chunk;
    }

    for (int i = centerChunk.x - VISIBLE_CHUNKS; i != centerChunk.x + VISIBLE_CHUNKS + 1; i++) {
        for (int j = centerChunk.z - VISIBLE_CHUNKS; j != centerChunk.z + VISIBLE_CHUNKS + 1; j++) {
            if (isChunkInLoadRange({ i,j }, centerChunk)) {
                Chunk * chunk = getChunk({ i,j });
                if (!chunk) {
                    chunk = new Chunk(this, perlin, ivec2(i, j), blocks, graphics, shader);
                    chunks.insert({ { i,j }, chunk });
                    chunk->state = ChunkState::InLoadingQueue;
                    chunk->updatePriority = abs(i - centerChunk.x) + abs(j - centerChunk.z);
                    loadJobs.push_front(chunk);

                    loadWaiter.notify_one();
                } else if (chunk->meshDirty && (chunk->state == ChunkState::Loaded || chunk->state == ChunkState::Meshed)&& isChunkNeighborsLoaded({ i,j })) {
                        chunk->state = ChunkState::InMeshingQueue;
                        chunk->updatePriority = abs(i - centerChunk.x) + abs(j - centerChunk.z);
                        meshJobs.push_front(chunk);
                        meshWaiter.notify_one();
                    }


            }
        }
    }

    while (!finishedLoadJobs.empty()){
        Chunk* job = finishedLoadJobs.front();
        finishedLoadJobs.pop();

        job->state = ChunkState::Loaded;
        job->meshDirty = true;
    }

    while (!finishedMeshJobs.empty()){
        Chunk* job = finishedMeshJobs.front();
        finishedMeshJobs.pop();

        job->state = ChunkState::Meshed;
        job->meshDirty = false;

        job->meshLength = job->tempMeshLength;

        job->updateBuffer();
    }

}
void World::renderChunks()
{
	for (auto const& chunk : chunks) {
		chunk.second->render();
	}
}

Chunk * World::getChunk(ivec2 coords)
{
	auto element = chunks.find(coords);
	if (element != chunks.end())
		return chunks[coords];
	return NULL;
}

ivec2 World::getChunkCoords(fvec3 worldPosition)
{
	return{ worldPosition.x / CHUNK_SIZE_X, worldPosition.z / CHUNK_SIZE_Z };
}
ivec2 World::getChunkCoords(ivec2 worldPosition)
{
	return{ (float)worldPosition.x / (float)CHUNK_SIZE_X, (float)worldPosition.z / (float)CHUNK_SIZE_Z };
}

bool World::isChunkInLoadRange(ivec2 chunkCoords, ivec2 playerCoords)
{
	return pow(chunkCoords.x - playerCoords.x, 2) + pow(chunkCoords.z - playerCoords.z, 2) < pow(VISIBLE_CHUNKS, 2);
}

bool World::isChunkInMeshRange(ivec2 chunkCoords, ivec2 playerCoords)
{
	return pow(chunkCoords.x - playerCoords.x, 2) + pow(chunkCoords.z - playerCoords.z, 2) < pow(VISIBLE_CHUNKS - 1, 2);
}

bool World::isChunkNeighborsLoaded(ivec2 chunkCoords)
{
	Chunk *chunk;
	chunkCoords += adjecent_chunk[0];
	chunk = getChunk(chunkCoords);
	if (!chunk || !chunk->isLoaded())
		return false;
	chunkCoords += adjecent_chunk[1];
	chunk = getChunk(chunkCoords);
	if (!chunk || !chunk->isLoaded())
		return false;
	chunkCoords += adjecent_chunk[2];
	chunk = getChunk(chunkCoords);
	if (!chunk || !chunk->isLoaded())
		return false;
	chunkCoords += adjecent_chunk[3];
	chunk = getChunk(chunkCoords);
	if (!chunk || !chunk->isLoaded())
		return false;
	return true;
}

void World::addLight(fvec3 position, int lightLevel)
{
	std::queue<lightPropogationNode> lightQueue;

	setLightLevel(position, lightLevel);
	lightQueue.push({ position, lightLevel });

	while (!lightQueue.empty()) {
		lightPropogationNode n = lightQueue.front();
		lightQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; i++) {
			newPos = n.position + adjecent[i];
			if (getBlock(newPos) == 0 && getLightLevel(newPos) < n.lightLevel - 1) {
				setLightLevel(newPos, n.lightLevel - 1);
				lightQueue.push({ newPos, n.lightLevel - 1 });
			}
		}
	}


}

void World::removeLight(fvec3 position)
{
	std::queue<lightPropogationNode> lightQueue;
	std::queue<lightPropogationNode> removalQueue;

	removalQueue.push({ position, getLightLevel(position) });
	setLightLevel(position, 0);

	while (!removalQueue.empty()) {
		lightPropogationNode n = removalQueue.front();
		removalQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; i++) {
			newPos = n.position + adjecent[i];
			unsigned short newLight = getLightLevel(newPos);
				if (newLight < n.lightLevel) {
					if (getBlock(newPos) == 0 && newLight > 0) {
						removalQueue.push({ newPos, newLight });
						setLightLevel(newPos, 0);
					}
				}
				else
					lightQueue.push({ newPos, newLight });
			
		}
	}
	
	while (!lightQueue.empty()) {
		lightPropogationNode n = lightQueue.front();
		lightQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; i++) {
			newPos = n.position + adjecent[i];
			if (getBlock(newPos) < 1 && getLightLevel(newPos) < n.lightLevel - 1) {
				setLightLevel(newPos, n.lightLevel - 1);
				lightQueue.push({ newPos, n.lightLevel - 1 });
			}
		}
	}

	removalQueue.push({ position, getSunlightLevel(position) });
	setSunlightLevel(position, 0);

	while (!removalQueue.empty()) {
		lightPropogationNode n = removalQueue.front();
		removalQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; i++) {
			newPos = n.position + adjecent[i];
			unsigned short newLight = getSunlightLevel(newPos);
			if (i != 4) {
				if (newLight < n.lightLevel) {
					if (getBlock(newPos) == 0 && newLight > 0) {
						removalQueue.push({ newPos, newLight });
						setSunlightLevel(newPos, 0);
					}
				}
				else {
					lightQueue.push({ newPos, newLight });
				}
			}else if (newLight <= n.lightLevel) {
				if (getBlock(newPos) == 0 && newLight > 0) {
					removalQueue.push({ newPos, newLight });
					setSunlightLevel(newPos, 0);
				}
			}
			else {
				lightQueue.push({ newPos, newLight });
			}
		}
	}

	while (!lightQueue.empty()) {
		lightPropogationNode n = lightQueue.front();
		lightQueue.pop();

		fvec3 newPos;
		for (int i = 0; i < 6; i++) {
			newPos = n.position + adjecent[i];
			if (i != 4) {
				if (getBlock(newPos) < 1 && getSunlightLevel(newPos) < n.lightLevel - 3) {
					setSunlightLevel(newPos, n.lightLevel - 1);
					lightQueue.push({ newPos, n.lightLevel - 1 });
				}
			}
			else if (getBlock(newPos) < 1 && getSunlightLevel(newPos) < n.lightLevel) {
				setSunlightLevel(newPos, n.lightLevel);
				lightQueue.push({ newPos, n.lightLevel });
			}
		}
	}
}

void World::addCompleteJobToQueue(std::queue<Chunk*> *q, Chunk* job){
    std::unique_lock<std::mutex> lock(m);
    q->push(job);
}

void World::meshTask()
{
	while (true) {
		Chunk* job = getNextMeshJob();
		job->genMesh();
        addCompleteJobToQueue(&finishedMeshJobs, job);
	}
}

Chunk * World::getNextMeshJob()
{
	std::unique_lock<std::mutex> lock(m);

	while (true)
	{
		if (!meshJobs.empty())
		{
			Chunk* job = meshJobs.front();
			meshJobs.pop_front();
            job->state = ChunkState::CurrentlyMeshing;
			return job;
		}

		meshWaiter.wait(lock);
	}
}

void World::loadTask()
{
    while (true) {
        Chunk* job = getNextLoadJob();
        job->loadData();
        addCompleteJobToQueue(&finishedLoadJobs, job);

    }
}

Chunk * World::getNextLoadJob()
{
    std::unique_lock<std::mutex> lock(m);

    while (true)
    {
        if (!loadJobs.empty())
        {
            Chunk* job = loadJobs.front();
            loadJobs.pop_front();
            job->state = ChunkState::CurrentlyLoading;
            return job;
        }

        loadWaiter.wait(lock);
    }
}

void World::physTask()
{
    while (true) {
        Box* box = getNextPhysObj();
        //box->loadData();
    }
}

Box *World::getNextPhysObj()
{
    std::unique_lock<std::mutex> lock(physMutex);

    while (true)
    {
        if (!boxes.empty())
        {
            Box* box = boxes.front();
            boxes.pop_front();
            return box;
        }

        loadWaiter.wait(lock);
    }
}

void World::createBoxes(int shaderId) {
    for (int i = 0; i < 10; ++i){
        AABB a;
        a.setPosition({i * 1, i * 1, 100});
        a.setSize({1, 1, 2});

        Box *b = new Box(graphics, a, shaderId);
        boxes.push_back(b);
    }
}

void World::renderBoxes(glm::mat4 *PV) {
    for (auto& b : boxes){
        b->render(PV);
    }
}

void World::createRandomBoxes(int shaderId){
    for (int i = 0; i < 100; ++i){
        AABB a;
        a.setPosition({std::rand() % 300 - 150, std::rand() % 100 - 50, 100 + std::rand() % 30});
        a.setSize({1, 1, 1 + std::rand() % 3});

        Box *b = new Box(graphics, a, shaderId);
        boxes.push_back(b);
    }
}



#include "worldgeneratorcomponent.h"


WorldGeneratorComponent::WorldGeneratorComponent(std::string worldName, TerrainType terrainType, GameObject* parent)
    : Component(parent) {

    this->_name = "WorldGeneratorComponent";
    this->_worldName = worldName;
    this->_terrainType = terrainType;

    this->_lastUpdate_CameraChunkPos = calculateGameObjectChunkPos(Camera::ActiveCamera);
}

void WorldGeneratorComponent::Start() {

    /** Initialize world generation **/

    std::string className = " (WorldGeneratorComponent)";

    std::cout << "Generating world..." << className << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{_seed};

    /** Get camera coordinates **/

    QVector3D cameraChunkPos = calculateGameObjectChunkPos(Camera::ActiveCamera);

    int x = 0;
    int y = 0;
    int z = 0;

    int xOffset = cameraChunkPos.x();
    int yOffset = cameraChunkPos.y();
    int zOffset = cameraChunkPos.z();

    int chunksCount = 0;

    /** Create the chunks **/

    chunksCount += addToChunksToGenerate(x, y, z);

    while (true) {
        /** Front to right **/
        z++;
        while (z > 0) {
            chunksCount += addToChunksToGenerate(x + xOffset, y + yOffset, z + zOffset);
            x++;
            z--;
        }

        /** Right to back **/
        while (x > 0) {
            chunksCount += addToChunksToGenerate(x + xOffset, y + yOffset, z + zOffset);
            x--;
            z--;
        }

        /** Back to left **/
        while (z < 0) {
            chunksCount += addToChunksToGenerate(x + xOffset, y + yOffset, z + zOffset);
            x--;
            z++;
        }

        /** Left to front **/
        while (x < 0) {
            chunksCount += addToChunksToGenerate(x + xOffset, y + yOffset, z + zOffset);
            x++;
            z++;
        }

        int minDist = sqrt(pow(z/2.0, 2) * 2);
        if (minDist > Camera::ActiveCamera->getRenderDistance())
            break;
    }

//    std::vector<QThread*> threads;

//    // WHILE LOOP TO DO FOR THREADS
//    {
//        generateChunk(x, y, z, this);
//        QThread* chunkGeneratorThread = QThread::create(WorldGeneratorComponent::generateChunk, x, y, z, this);

//        chunkGeneratorThread->setObjectName(("Chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + "_GeneratorThread").c_str());
//        chunkGeneratorThread->start();

//        threads.push_back(chunkGeneratorThread);
//    }


//    int threadCount = 0;
//    for (auto* thread : threads) {
//        thread->wait();
//        std::cout << "\rThread: " << threadCount++ << " has finished" << std::flush;
//    }
//    std::cout << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "World generation done (created " << chunksCount << " chunks) in " << elapsed.count() << "s" << className << std::endl;
}

void WorldGeneratorComponent::Update(float delta) {
    updateChunksToGenerate();

    /** Generate chunks using the quota **/

    if (!_chunksToGenerate.empty()) {
        int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToGenerate.size());
        while (remainingQuota-- > 0) {
            QVector3D* chunkPos = _chunksToGenerate.pop_front();
            std::cout << "Generating chunk... " << chunkPos->x() << " " << chunkPos->y() << " " << chunkPos->z() << std::endl;
            generateChunk(chunkPos->x(), chunkPos->y(), chunkPos->z(), this);
            delete chunkPos;
        }
    }

    /** Finalize chunks using the quota **/

    if (!_chunksToFinalize.empty()) {
        int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToFinalize.size());
        while (remainingQuota-- > 0) {
            GameObject* chunk = _chunksToFinalize.pop_front();
            std::cout << "Finalizing " << chunk->_name << std::endl;
            finalizeChunkCreation(chunk);
        }
    }
}

void WorldGeneratorComponent::updateChunksToGenerate() {
    QVector3D currentUpdate_CameraChunkPos = calculateGameObjectChunkPos(Camera::ActiveCamera);

    QVector3D deltaChunkPos = _lastUpdate_CameraChunkPos - currentUpdate_CameraChunkPos;

    if ((int)(deltaChunkPos.x()) != 0)
        std::cout << "Moved from chunk x = " << (int)(_lastUpdate_CameraChunkPos.x()) << " to " << (int)(currentUpdate_CameraChunkPos.x()) << std::endl;
    if ((int)(deltaChunkPos.y()) != 0)
        std::cout << "Moved from chunk y = " << (int)(_lastUpdate_CameraChunkPos.y()) << " to " << (int)(currentUpdate_CameraChunkPos.y()) << std::endl;
    if ((int)(deltaChunkPos.z()) != 0)
        std::cout << "Moved from chunk z = " << (int)(_lastUpdate_CameraChunkPos.z()) << " to " << (int)(currentUpdate_CameraChunkPos.z()) << std::endl;

    // TODO: addToChunksToGenerate()

    _lastUpdate_CameraChunkPos = currentUpdate_CameraChunkPos;
}

int WorldGeneratorComponent::addToChunksToGenerate(int x, int y, int z, bool compareToRenderDistance) {
    if (compareToRenderDistance) {
        QVector3D cameraChunkPos = calculateGameObjectChunkPos(Camera::ActiveCamera);
        int distanceToCameraChunk = sqrt(pow(x - cameraChunkPos.x(), 2) + pow(z - cameraChunkPos.z(), 2)); // TODO: take y into account if vertical chunks are implemented
        if (distanceToCameraChunk > Camera::ActiveCamera->getRenderDistance()) return 0;
    }
    this->_chunksToGenerate.push_back(new QVector3D(x, y, z));
    return 1;
}

void WorldGeneratorComponent::addToChunksToFinalize(GameObject* chunk) {
    this->_chunksToFinalize.push_back(chunk);
}

QVector3D WorldGeneratorComponent::calculateGameObjectChunkPos(GameObject* gameObject) {
    QVector3D gameObjectPos = gameObject->GetTransform()->GetPosition();

    QVector3D chunkPos;
    chunkPos.setX((int)(gameObjectPos.x() / _CHUNK_X_SIZE));
    chunkPos.setY((int)(gameObjectPos.y() / _CHUNK_Y_SIZE));
    chunkPos.setZ((int)(gameObjectPos.z() / _CHUNK_Z_SIZE));

    return chunkPos;
}

void WorldGeneratorComponent::generateChunk(int x, int y, int z, WorldGeneratorComponent* WGC) {

    std::string chunkName = "Chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);

    /** Create the chunk GameObject **/

    GameObject* chunk = new GameObject(chunkName, WGC->GetParent(), false); /* /!\ Ensure that the chunk GameObject is not _enabled /!\ */
    chunk->GetTransform()->SetPosition(x * WGC->_CHUNK_X_SIZE, y * WGC->_CHUNK_Y_SIZE, z * WGC->_CHUNK_Z_SIZE);

    /** Create and attach its terrain generation related Components **/

    new OctreeComponent(WGC->_CHUNK_X_SIZE, WGC->_CHUNK_Y_SIZE, WGC->_CHUNK_Z_SIZE, WGC->_CHUNK_NB_OF_LAYERS, WGC->_CHUNK_LAYER_SIZE_REDUCTION_FACTOR, chunk);

    switch (WGC->_terrainType) {
    case TerrainType::PERLIN_2D:
        new Perlin2dTerrainComponent(x, y, z, WGC->_perlin, WGC->_octaves, WGC->_frequency, WGC->_persistence, WGC->_roughness, chunk);
        break;
    case TerrainType::PERLIN_3D:
        std::cerr << "PERLIN_3D terrain generation not implemented yet, aborting world creation" << std::endl;
        return;
    case TerrainType::FLAT:
        std::cerr << "FLAT terrain generation not implemented yet, aborting world creation" << std::endl;
        return;
    case TerrainType::FLOATING_ISLANDS:
        std::cerr << "FLOATING_ISLANDS terrain generation not implemented yet, aborting world creation" << std::endl;
        return;
    case TerrainType::BIG_CAVES:
        std::cerr << "BIG_CAVES terrain generation not implemented yet, aborting world creation" << std::endl;
        return;
    default:
        std::cerr << "Unknown terrain generation specified, aborting world creation" << std::endl;
        return;
    }

    // NOTE: dont forget to uncomment
    chunk->SetCollider(new OctreeCollider());

    /** Add the chunk to the LinkedQueue of chunks to send to the GPU **/

    WGC->addToChunksToFinalize(chunk);

//    std::cout << chunk->_name << " has been generated." << std::endl;
}

void WorldGeneratorComponent::finalizeChunkCreation(GameObject* chunk) {
    new OctreeRendererComponent(chunk);
    chunk->Enable();
}

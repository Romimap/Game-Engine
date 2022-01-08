#include "worldgeneratorcomponent.h"


WorldGeneratorComponent::WorldGeneratorComponent(std::string worldName, TerrainType terrainType, GameObject* parent)
    : Component(parent) {

    this->_name = "WorldGeneratorComponent";
    this->_worldName = worldName;
    this->_terrainType = terrainType;
}

void WorldGeneratorComponent::Start() {
    std::string className = " (WorldGeneratorComponent)";

    std::cout << "Generating world..." << className << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{_seed};

    int xMin = -Camera::ActiveCamera->getRenderDistance(), xMax = Camera::ActiveCamera->getRenderDistance();
    int yMin = 0, yMax = 0; // Don't touch until vertical chunks are implemented! y should always = 0
    int zMin = -Camera::ActiveCamera->getRenderDistance(), zMax = Camera::ActiveCamera->getRenderDistance();

    int chunksCount = 0;
    int totalChunks = (xMax - xMin + 1) * (yMax - yMin + 1) * (zMax - zMin + 1);

    /** Create the chunks **/

//    std::vector<QThread*> threads;

    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            for (int z = zMin; z <= zMax; z++) {
//                QThread* chunkGeneratorThread = QThread::create(WorldGeneratorComponent::generateChunk, x, y, z, this);
                generateChunk(x, y, z, this);

//                chunkGeneratorThread->setObjectName(("Chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + "_GeneratorThread").c_str());
//                chunkGeneratorThread->start();

//                threads.push_back(chunkGeneratorThread);

                // std::future<void> fut = std::async(WorldGeneratorComponent::generateChunk, x, y, z, this);

                chunksCount++;

                int percentage = (100 * chunksCount) / totalChunks;
                std::cout << "\rProgress: " << chunksCount << "/" << totalChunks << " chunks (" << percentage << "%)" << className << std::flush;
            }
        }
    }
    std::cout << std::endl;

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
    if (_chunksToFinalize.empty()) return;

    std::cout << "### Has " << _chunksToFinalize.size() << " chunks to finalize ###" << std::endl;

    int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToFinalize.size());
    while (remainingQuota-- > 0) {
        GameObject* chunk = _chunksToFinalize.pop_front();
        finalizeChunkCreation(chunk);
    }
}

void WorldGeneratorComponent::addToChunksToFinalize(GameObject* chunk) {
    this->_chunksToFinalize.push_back(chunk);
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
    // chunk->SetCollider(new OctreeCollider());

    /** Add the chunk to the LinkedQueue of chunks to send to the GPU **/

    WGC->addToChunksToFinalize(chunk);

    std::cout << chunk->_name << " has been generated." << std::endl;
}

void WorldGeneratorComponent::finalizeChunkCreation(GameObject* chunk) {
    new OctreeRendererComponent(chunk);
    chunk->Enable();
}

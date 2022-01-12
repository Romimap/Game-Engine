#include "worldgeneratorcomponent.h"


// --------------------------------------------------------------------------------
// CONSTRUCTORS/DESTRUCTORS
// --------------------------------------------------------------------------------

WorldGeneratorComponent::WorldGeneratorComponent(std::string worldName, TerrainType terrainType, GameObject* parent)
    : Component(parent) {

    this->_name = "WorldGeneratorComponent";
    this->_worldName = worldName;
    this->_terrainType = terrainType;

    this->_lastUpdate_CameraChunkPos = getChunkPos(Camera::ActiveCamera);
}


// --------------------------------------------------------------------------------
// GENERIC COMPONENT METHODS
// --------------------------------------------------------------------------------

void WorldGeneratorComponent::Start() {

    /** Initialize world generation **/

    this->_perlin = siv::PerlinNoise{_seed};
    generateChunksAroundActiveCamera();
}

///Handles chunk creation and deletion @ each frame
void WorldGeneratorComponent::Update(float delta) {

    /** Verify if new chunks should be created or distant chunks be deleted **/

    updateChunksGen();

    /** Delete chunks using the quota **/

    if (!_chunksToDelete.empty()) {
        int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToDelete.size());
        while (remainingQuota-- > 0 && !_chunksToDelete.empty()) {
            std::string* chunkName = _chunksToDelete.pop_front();
            if (!deleteChunk(*chunkName)) {
                remainingQuota++;
            }
            delete chunkName;
        }
    }

    /** Generate chunks using the quota **/

    if (!_chunksToGenerate.empty()) {
        int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToGenerate.size());
        while (remainingQuota-- > 0) {
            QVector3D* chunkPos = _chunksToGenerate.pop_front();
//            std::cout << "Generating chunk... " << chunkPos->x() << " " << chunkPos->y() << " " << chunkPos->z() << std::endl;
            generateChunk(chunkPos->x(), chunkPos->y(), chunkPos->z(), this);
            delete chunkPos;
        }
    }

    /** Finalize chunks using the quota **/

    if (!_chunksToFinalize.empty()) {
        int remainingQuota = std::min(_CHUNKS_PER_UPDATE, _chunksToFinalize.size());
        while (remainingQuota-- > 0) {
            GameObject* chunk = _chunksToFinalize.pop_front();
//            std::cout << "Finalizing " << chunk->_name << std::endl;
            finalizeChunkCreation(chunk);
        }
    }
}


// --------------------------------------------------------------------------------
// OTHER METHODS
// --------------------------------------------------------------------------------

///Returns the voxel at <x, y, z>, on the layer <layerID>, layer 0 being the most detailed layer
unsigned char WorldGeneratorComponent::getVoxelType(int x, int y, int z, int layerID) {
    if (layerID != 0)
        std::cout << "Used with layerID != 0" << std::endl;
    GameObject* chunk = getChunkFromVoxelPos(x, y, z);
    if (chunk != nullptr) {
        QVector3D localVoxelPos = getLocalVoxelCoordinates(x, y, z, layerID);
        return chunk->GetComponent<OctreeComponent>()->getVoxelType(localVoxelPos.x(), localVoxelPos.y(), localVoxelPos.z(), layerID);
    }
    else {
//        std::cerr << "WorldGeneratorComponent::getVoxelType returning 'AIR' because no chunk was found" << std::endl;
        return MaterialId::AIR;
    }
}

///Sets the voxel at <x, y, z>, with the material <voxelMaterial>
int WorldGeneratorComponent::setVoxelType(int x, int y, int z, unsigned char voxelMaterial) {
    GameObject* chunk = getChunkFromVoxelPos(x, y, z);
    if (chunk != nullptr) {
        QVector3D localVoxelPos = getLocalVoxelCoordinates(x, y, z);
        return chunk->GetComponent<OctreeComponent>()->setVoxelType(localVoxelPos.x(), localVoxelPos.y(), localVoxelPos.z(), voxelMaterial);
    }
    else {
//        std::cerr << "WorldGeneratorComponent::setVoxelType returning '-1' because no chunk was found" << std::endl;
        return -1;
    }
}


// --------------------------------------------------------------------------------
// CHUNK GENERATION
// --------------------------------------------------------------------------------

///Generates chunks around the active camera
void WorldGeneratorComponent::generateChunksAroundActiveCamera() {

    std::string className = " (WorldGeneratorComponent)";

    auto start = std::chrono::high_resolution_clock::now();

    /** Get camera coordinates **/

    QVector3D cameraChunkPos = getChunkPos(Camera::ActiveCamera);

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


///Removes chunks based on their distance to the active camera.
void WorldGeneratorComponent::removeDistantChunks() {
    std::map<std::string, GameObject*> chunks = _parent->GetChildren();
    for (auto it = chunks.begin(); it != chunks.end(); it++) {
        GameObject* chunk = it->second;
        if (chunk == nullptr) {
            std::cout << "removeDistantChunks found a nullptr chunk" << std::endl;
        }
        QVector3D chunkPos = getChunkPos(chunk);

        int distanceToCameraChunk = getDistanceFromCameraToChunk(chunkPos.x(), 0, chunkPos.z()); // TODO: take y into account if vertical chunks are implemented
        std::cout << "Distance from camera to (" << chunkPos.x() << ", " << chunkPos.y() << ", " << chunkPos.z() << ") = " << distanceToCameraChunk << std::endl;
        if (distanceToCameraChunk > Camera::ActiveCamera->getRenderDistance()){
            addToChunksToDelete(chunk->_name);
        }
    }
}

///Called by Update(), Creates & Delete chunks
void WorldGeneratorComponent::updateChunksGen() {
    QVector3D currentUpdate_CameraChunkPos = getChunkPos(Camera::ActiveCamera);

    QVector3D deltaChunkPos = _lastUpdate_CameraChunkPos - currentUpdate_CameraChunkPos;

    if (deltaChunkPos.length() != 0) {
        auto start = std::chrono::high_resolution_clock::now();

        removeDistantChunks();
        generateChunksAroundActiveCamera();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "World update done in " << elapsed.count() << "s (WorldGeneratorComponent)" << std::endl;

        _lastUpdate_CameraChunkPos = currentUpdate_CameraChunkPos;
        std::cout << "Camera chunk pos is now: (" << _lastUpdate_CameraChunkPos.x() << ", " << _lastUpdate_CameraChunkPos.y() << ", " << _lastUpdate_CameraChunkPos.z() << ")" << std::endl;
    }
}

///Generates a new chunk
void WorldGeneratorComponent::generateChunk(int chunkX, int chunkY, int chunkZ, WorldGeneratorComponent* WGC) {

    std::string chunkName = getChunkNameFromChunkPos(chunkX, chunkY, chunkZ);

    /** Create the chunk GameObject **/

    GameObject* chunk = new GameObject(chunkName, WGC->GetParent(), false); /* /!\ Ensure that the chunk GameObject is not _enabled /!\ */
    chunk->GetTransform()->SetPosition(chunkX * WGC->_CHUNK_X_SIZE, chunkY * WGC->_CHUNK_Y_SIZE, chunkZ * WGC->_CHUNK_Z_SIZE);

    /** Create and attach its terrain generation related Components **/

    new OctreeComponent(WGC->_CHUNK_X_SIZE, WGC->_CHUNK_Y_SIZE, WGC->_CHUNK_Z_SIZE, WGC->_CHUNK_NB_OF_LAYERS, WGC->_CHUNK_LAYER_SIZE_REDUCTION_FACTOR, chunk);

    switch (WGC->_terrainType) {
    case TerrainType::PERLIN_2D:
        new Perlin2dTerrainComponent(chunkX, chunkY, chunkZ, WGC->_perlin, WGC->_octaves, WGC->_frequency, WGC->_persistence, WGC->_roughness, chunk);
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

///Called when the chunk is ready, adds a renderer to it and enables it
void WorldGeneratorComponent::finalizeChunkCreation(GameObject* chunk) {
    new OctreeRendererComponent(chunk);
    chunk->Enable();
}

///Deletes a chunk
bool WorldGeneratorComponent::deleteChunk(std::string chunkName) {
    GameObject* chunk = _parent->GetChildByName(chunkName);
    if (chunk == nullptr) {
        return false;
    }
    else {
        delete chunk;
        return true;
    }
}


// --------------------------------------------------------------------------------
// QUEUES MANAGEMENT
// --------------------------------------------------------------------------------
int WorldGeneratorComponent::addToChunksToGenerate(int chunkX, int chunkY, int chunkZ, bool compareToRenderDistance) {

    /** Verify is the chunk is already loaded, if so, return **/

    GameObject* chunk = _parent->GetChildByName(getChunkNameFromChunkPos(chunkX, 0, chunkZ)); // TODO: take y into account if vertical chunks are implemented
    if (chunk != nullptr)
        return 0;

    /** Verify the distance from the camera **/

    if (compareToRenderDistance) {
        int distanceToCameraChunk = getDistanceFromCameraToChunk(chunkX, 0, chunkZ);
        if (distanceToCameraChunk > Camera::ActiveCamera->getRenderDistance())
            return 0;
    }

    /** Set the chunk to be generated **/

    this->_chunksToGenerate.push_back(new QVector3D(chunkX, 0, chunkZ)); // TODO: take y into account if vertical chunks are implemented
    return 1;
}

void WorldGeneratorComponent::addToChunksToFinalize(GameObject* chunk) {
    this->_chunksToFinalize.push_back(chunk);
}

void WorldGeneratorComponent::addToChunksToDelete(std::string chunkName) {
    this->_chunksToDelete.push_back(new std::string(chunkName));
}


// --------------------------------------------------------------------------------
// OTHER METHODS
// --------------------------------------------------------------------------------

std::string WorldGeneratorComponent::getChunkNameFromChunkPos(int chunkX, int chunkY, int chunkZ) {
    return "Chunk_" + std::to_string(chunkX) + "_" + std::to_string(chunkY) + "_" + std::to_string(chunkZ);
}

GameObject* WorldGeneratorComponent::getChunkFromVoxelPos(int x, int y, int z) {
    QVector3D chunkPos = getChunkPos(x, y, z);
    std::string chunkName = getChunkNameFromChunkPos(chunkPos.x(), chunkPos.y(), chunkPos.z());

    GameObject* chunk = nullptr;
    std::map<std::string, GameObject*> children = this->GetParent()->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        if (child->_name == chunkName) {
            chunk = child;
            break;
        }
    }
    if (chunk == nullptr) {
        return nullptr;
    }
    else {
        return chunk;
    }
}

QVector3D WorldGeneratorComponent::getLocalVoxelCoordinates(int x, int y, int z, int layerID) {

    QVector3D localCoordinates;

    /** Calculate the moduluses, depending on the layerID **/

    int _currentLayerSizeReductionFactor = pow(_CHUNK_LAYER_SIZE_REDUCTION_FACTOR, layerID);
    int xSize = _CHUNK_X_SIZE / _currentLayerSizeReductionFactor;
    int ySize = _CHUNK_Y_SIZE / _currentLayerSizeReductionFactor;
    int zSize = _CHUNK_Z_SIZE / _currentLayerSizeReductionFactor;

    /** Calculate the local coordinates of the voxel, depending on positive or negative coordinates **/

    if (x < 0) localCoordinates.setX((x + 1) % xSize + 63);
    else localCoordinates.setX(x % xSize);
    if (y < 0) localCoordinates.setY((y + 1) % ySize + 63);
    else localCoordinates.setY(y % ySize);
    if (z < 0) localCoordinates.setZ((z + 1) % zSize + 63);
    else localCoordinates.setZ(z % zSize);

    return localCoordinates;
}

QVector3D WorldGeneratorComponent::getChunkPos(GameObject* gameObject) { // TODO: INVESTIGATE
    QVector3D gameObjectPos = gameObject->GetTransform()->GetPosition();
    return getChunkPos(gameObjectPos.x(), gameObjectPos.y(), gameObjectPos.z());
}

QVector3D WorldGeneratorComponent::getChunkPos(int x, int y, int z) {

    /** Account for negative chunks coordinates particularities **/

    if (x < 0)
        x -= 63;
    if (y < 0)
        y -= 63;
    if (z < 0)
        z -= 63;

    /** Calculate the chunk position **/

    QVector3D chunkPos;
    chunkPos.setX((int)(x / _CHUNK_X_SIZE));
    chunkPos.setY((int)(y / _CHUNK_Y_SIZE));
    chunkPos.setZ((int)(z / _CHUNK_Z_SIZE));

    return chunkPos;
}

float WorldGeneratorComponent::getDistanceFromCameraToChunk(int chunkX, int chunkY, int chunkZ) {
    QVector3D cameraChunkPos = getChunkPos(Camera::ActiveCamera);
    return sqrt(pow(chunkX - cameraChunkPos.x(), 2) + pow(chunkZ - cameraChunkPos.z(), 2)); // TODO: take y into account if vertical chunks are implemented
}

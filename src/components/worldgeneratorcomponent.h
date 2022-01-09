#ifndef WORLDGENERATORCOMPONENT_H
#define WORLDGENERATORCOMPONENT_H


#include <chrono>
#include <future>
#include <iostream>

#include <QThread>
#include <QVector3D>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/consts.h"
#include "src/gameobject.h"
#include "src/linkedqueue.h"
#include "src/octreecollider.h"

#include "src/components/octreerenderercomponent.h"
#include "src/components/terrain/octreecomponent.h"
#include "src/components/terrain/perlin2dterraincomponent.h"


class WorldGeneratorComponent : public Component {
    /*** ATTRIBUTES ***/
public:
    std::string _worldName;

    /** World gen related **/
    siv::PerlinNoise::seed_type _seed = 123456u;  // [0 ... 2^32-1]

    const int _octaves = 8;             // [1 ... 16]
    const float _frequency = .8f;      // [0.1 ... 64.0]
    const float _persistence = 0.5f;    // [0.0 ... 1.0]
    const float _roughness = .3f;      // [0.0 ... 10.0]

protected:
    /** World gen related **/
    TerrainType _terrainType;

    siv::PerlinNoise _perlin;

    QVector3D _lastUpdate_CameraChunkPos;

    /** Chunks related **/
    const int _CHUNK_X_SIZE = 64;
    const int _CHUNK_Y_SIZE = 256;
    const int _CHUNK_Z_SIZE = 64;

    const int _CHUNK_NB_OF_LAYERS = 3;
    const int _CHUNK_LAYER_SIZE_REDUCTION_FACTOR = 4;

    /** Chunk generation queues **/
    const int _CHUNKS_PER_UPDATE = 1;

    LinkedQueue<QVector3D*> _chunksToGenerate;
    LinkedQueue<GameObject*> _chunksToFinalize;
    LinkedQueue<GameObject*> _chunksToDelete;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    WorldGeneratorComponent(std::string worldName, TerrainType terrainType, GameObject* parent);

    /** GENERIC COMPONENT METHODS */
    void Start() override;
    void Update(float delta) override;

    /** OTHER METHODS **/
    unsigned char getVoxelType(int x, int y, int z, int layerID = 0);
    int setVoxelType(int x, int y, int z, unsigned char voxelMaterial);

protected:
    /** CHUNK GENERATION **/
    void generateChunksAroundActiveCamera();
    void removeDistantChunks();

    void updateChunksGen();

    static void generateChunk(int chunkX, int chunkY, int chunkZ, WorldGeneratorComponent* WGC);
    void finalizeChunkCreation(GameObject* chunk);

    /** QUEUES MANAGEMENT **/
    int addToChunksToGenerate(int chunkX, int chunkY, int chunkZ, bool compareToRenderDistance = true);
    void addToChunksToFinalize(GameObject* chunk);
    void addToChunksToDelete(GameObject* chunk);

    /** OTHER METHODS **/
    static std::string getChunkNameFromChunkPos(int chunkX, int chunkY, int chunkZ);

    GameObject* getChunkFromVoxelPos(int x, int y, int z);

    QVector3D calculateChunkPos(GameObject* gameObject);
    QVector3D calculateChunkPos(int x, int y, int z);

    float calculateDistanceFromCameraToChunk(int chunkX, int chunkY, int chunkZ);

    /** CHUNK GENERATION **/
};

#endif // WORLDGENERATORCOMPONENT_H

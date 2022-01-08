#ifndef WORLDGENERATORCOMPONENT_H
#define WORLDGENERATORCOMPONENT_H


#include <chrono>
#include <future>
#include <iostream>

#include <QThread>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/consts.h"
#include "src/gameobject.h"
#include "src/octreecollider.h"

#include "src/components/chunkfinalizercomponent.h"
#include "src/components/octreerenderercomponent.h"
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

    /** Chunks related **/
    const int _CHUNK_X_SIZE = 64;
    const int _CHUNK_Y_SIZE = 256;
    const int _CHUNK_Z_SIZE = 64;

    const int _CHUNK_NB_OF_LAYERS = 3;
    const int _CHUNK_LAYER_SIZE_REDUCTION_FACTOR = 4;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    WorldGeneratorComponent(std::string worldName, TerrainType terrainType, GameObject* parent);

    /** GENERIC COMPONENT METHODS */
    void Start() override;

    /** GETTERS/SETTERS **/
    TerrainType getTerrainType();
    siv::PerlinNoise getPerlinNoise();
    int getChunkXSize();
    int getChunkYSize();
    int getChunkZSize();
    int getChunkNbOfLayers();
    int getChunkLayerSizeReductionFactor();

protected:
    /** CHUNK GENERATION **/
    static void generateChunk(int x, int y, int z, WorldGeneratorComponent* WGC);
};

#endif // WORLDGENERATORCOMPONENT_H

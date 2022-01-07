#ifndef WORLDGENERATORCOMPONENT_H
#define WORLDGENERATORCOMPONENT_H


#include <chrono>
#include <iostream>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/consts.h"
#include "src/gameobject.h"

#include "src/components/terrain/perlin2dterraincomponent.h"


using namespace std;


class WorldGeneratorComponent : public Component {
    /*** ATTRIBUTES ***/
public:
    string _worldName;

protected:
    /** World gen related **/
    siv::PerlinNoise::seed_type _seed;  // [0 ... 2^32-1]

    TerrainType _terrainType;

    siv::PerlinNoise _perlin;

    const int _OCTAVES = 8;             // [1 ... 16]
    const float _FREQUENCY = .8f;      // [0.1 ... 64.0]
    const float _PERSISTENCE = 0.5f;    // [0.0 ... 1.0]
    const float _ROUGHNESS = .3f;      // [0.1 ... 10.0]

    const unsigned int TERRAIN_GEN_SQUARE_RADIUS = 0;

    /** Chunks related **/
    const int _CHUNK_X_SIZE = 64;
    const int _CHUNK_Y_SIZE = 256;
    const int _CHUNK_Z_SIZE = 64;

    const int _CHUNK_NB_OF_LAYERS = 3;
    const int _CHUNK_LAYER_SIZE_REDUCTION_FACTOR = 4;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    WorldGeneratorComponent(string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent);

    /** CHUNK GENERATION **/
    bool generateChunk(int x, int z);
};

#endif // WORLDGENERATORCOMPONENT_H

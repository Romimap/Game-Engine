#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H


#include <chrono>
#include <iostream>

#include "../libraries/PerlinNoise/PerlinNoise.hpp"
#include "chunk.h"


using namespace std;


class WorldGenerator {
    /*** ATTRIBUTES ***/
public:
    string _worldName;

    /** World gen related **/
    siv::PerlinNoise::seed_type _seed;   // [0 ... 2^32-1]

    siv::PerlinNoise _perlin;

    const int _OCTAVES = 8;              // [1 ... 16]
    const float _FREQUENCY = 8.0f;       // [0.1 ... 64.0]
    const float _PERSISTENCE = 0.5f;     // [0.0 ... 1.0]

    /** Chunks related **/
    const int _CHUNK_X_SIZE = 64;
    const int _CHUNK_Y_SIZE = 256;
    const int _CHUNK_Z_SIZE = 64;


    /*** CONSTRUCTORS/DESTRUCTORS ***/
    WorldGenerator(string worldName, siv::PerlinNoise::seed_type seed);


    /*** METHODS ***/
    bool genChunk(int x, int z);
};

#endif // WORLDGENERATOR_H

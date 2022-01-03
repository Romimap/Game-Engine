#ifndef CHUNK_H
#define CHUNK_H


#include <cmath>
#include <iostream>
#include <vector>

#include "voxelmaterial.h"
#include "../libraries/PerlinNoise/PerlinNoise.hpp"


using namespace std;


class Chunk {
    /*** ATTRIBUTES ***/
private:
    /** Update method related **/
    bool _hasChanged = true;

    /** Chunk data **/
    int _xSize;
    int _ySize;
    int _zSize;

    vector<unsigned char ***> _layers;
    int _precisionFactor;

public:
    /** Position **/
    int _x;
    int _y;
    int _z;


    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Chunk(int x, int y, int z, int xSize, int ySize, int zSize, int nbOfLayers, int precisionFactor, const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence);
    ~Chunk();

    /** UPDATE **/
    void update();

    /** GETTERS/SETTERS **/
    unsigned char getVoxel(int x, int y, int z);
    int setVoxel(int x, int y, int z, unsigned char voxelMaterial);

    /** DEBUG **/
    void printLayer(int layer);
};

#endif // CHUNK_H

#ifndef TERRAINCOMPONENT_H
#define TERRAINCOMPONENT_H


#include <cmath>
#include <iostream>
#include <vector>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/gameobject.h"
#include "src/consts.h"


using namespace std;


class TerrainComponent : public Component {
    /*** ATTRIBUTES ***/
protected:
    /** Update method related **/
    bool _hasChanged = true;

    /** Chunk data **/
    int _xSize;
    int _ySize;
    int _zSize;

    vector<vector<vector<vector<unsigned char>>>> _layers;
    int _layerSizeReductionFactor;

    /** Position **/
    int _chunkX;
    int _chunkY;
    int _chunkZ;


    /*** METHODS ***/
protected:
    /** CONSTRUCTORS/DESTRUCTORS **/
    TerrainComponent(int chunkX, int chunkY, int chunkZ, int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor, GameObject* parent);

public:
    /** UPDATE **/
    void update();

    /** GETTERS/SETTERS **/
    unsigned char getVoxelType(int x, int y, int z, int layerID = 0);
    int setVoxelType(int x, int y, int z, unsigned char voxelMaterial, int layerID = 0);

    vector<vector<vector<unsigned char>>> getLayer(int layerID);

    /** DEBUG **/
    void debugPrintLayers();
};

#endif // TERRAINCOMPONENT_H

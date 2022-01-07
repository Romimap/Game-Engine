#ifndef OCTREECOMPONENT_H
#define OCTREECOMPONENT_H


#include <cmath>
#include <iostream>
#include <vector>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/gameobject.h"
#include "src/consts.h"


using namespace std;


class OctreeComponent : public Component {
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


    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    OctreeComponent(int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor, GameObject* parent);

public:
    /** UPDATE **/
    void update();

    /** GETTERS/SETTERS **/
    unsigned char getVoxelType(int x, int y, int z, int layerID = 0);
    int setVoxelType(int x, int y, int z, unsigned char voxelMaterial, int layerID = 0);
    int getXSize() {return _xSize;}
    int getYSize() {return _ySize;}
    int getZSize() {return _zSize;}
    int getLayerSizeReductionFactor() {return _layerSizeReductionFactor;}
    int getNumberOfLayers();
    vector<vector<vector<unsigned char>>> getLayer(int layerID);
    vector<vector<vector<vector<unsigned char>>>> *getLayers();

    /** DEBUG **/
    void debugPrintLayers();

};

#endif // OCTREECOMPONENT_H

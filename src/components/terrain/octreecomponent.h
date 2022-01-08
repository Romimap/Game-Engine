#ifndef OCTREECOMPONENT_H
#define OCTREECOMPONENT_H


#include <cmath>
#include <iostream>
#include <vector>

#include "libraries/PerlinNoise/PerlinNoise.hpp"

#include "src/component.h"
#include "src/consts.h"
#include "src/gameobject.h"
#include "src/components/octreerenderercomponent.h"
#include "src/linkedqueue.h"


struct OctreeRendererChange;
class OctreeComponent : public Component {
    /*** ATTRIBUTES ***/
protected:
    /** Update method related **/
    bool _hasChanged = true;

    /** Chunk data **/
    int _xSize;
    int _ySize;
    int _zSize;

    LinkedQueue<OctreeRendererChange*> list4;
    LinkedQueue<OctreeRendererChange*> list16;
    LinkedQueue<OctreeRendererChange*> list64;

    std::vector<std::vector<std::vector<std::vector<unsigned char>>>> _layers;
    int _layerSizeReductionFactor;


    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    OctreeComponent(int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor, GameObject* parent);

public:
    /** UPDATE **/
    void Update(float delta) override;

    /** GETTERS/SETTERS **/
    unsigned char getVoxelType(int x, int y, int z, int layerID = 0);
    int setVoxelType(int x, int y, int z, unsigned char voxelMaterial);
    int getXSize() {return _xSize;}
    int getYSize() {return _ySize;}
    int getZSize() {return _zSize;}
    int getLayerSizeReductionFactor() {return _layerSizeReductionFactor;}
    int getNumberOfLayers();
    std::vector<std::vector<std::vector<unsigned char>>> getLayer(int layerID);
    std::vector<std::vector<std::vector<std::vector<unsigned char>>>> *getLayers();

    /** DEBUG **/
    void debugPrintLayers();

};

#endif // OCTREECOMPONENT_H

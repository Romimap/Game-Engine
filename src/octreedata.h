#ifndef OCTREEDATA_H
#define OCTREEDATA_H

#include "libraries/octree/octree.h"
#include <QOpenGLTexture>

//a simple wrapper

struct OctreeSpaceValues {
    char material;

    OctreeSpaceValues(char i) {
        material = i;
    }
};

struct OctreeRenderData {
};

class OctreeData {
public:
    Octree<OctreeSpaceValues>* spaceValues;
public:
    OctreeData(int size);
    ~OctreeData();
};

#endif // OCTREEDATA_H

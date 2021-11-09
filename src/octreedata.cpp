#include "octreedata.h"


OctreeData::OctreeData(int size) {
    spaceValues = new Octree<OctreeSpaceValues>(size);
}

OctreeData::~OctreeData() {
    delete spaceValues;
}

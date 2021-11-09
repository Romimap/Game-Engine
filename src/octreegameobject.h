#ifndef OCTREEGAMEOBJECT_H
#define OCTREEGAMEOBJECT_H

#include "gameobject.h"
#include "octreedata.h"
#include "octreecollider.h"

class OctreeGameObject : public GameObject {
protected:
    OctreeData* _octree;
    OctreeRenderData* _renderData;
public:
    OctreeGameObject(int size);
};

#endif // OCTREEGAMEOBJECT_H

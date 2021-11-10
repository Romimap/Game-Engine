#ifndef OCTREECOLLIDER_H
#define OCTREECOLLIDER_H

#include "gameobject.h"
#include "collider.h"
#include "octreedata.h"
#include "octreegameobject.h"

class OctreeCollider : public Collider {

private:
    OctreeData* _octree;

public:
    OctreeCollider(OctreeData* octree);
};

#endif // OCTREECOLLIDER_H

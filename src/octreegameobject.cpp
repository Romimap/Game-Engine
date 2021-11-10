#include "octreegameobject.h"

OctreeGameObject::OctreeGameObject(int size) : GameObject() {
    _octree = new OctreeData(size);
    SetCollider(new OctreeCollider(_octree));
}

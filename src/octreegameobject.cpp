#include "octreegameobject.h"

OctreeGameObject::OctreeGameObject(int size) : GameObject() {
    _octree = new OctreeData(size);
    _collider = new OctreeCollider(this, _octree);
}

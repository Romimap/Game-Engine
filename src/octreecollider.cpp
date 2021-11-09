#include "octreecollider.h"

OctreeCollider::OctreeCollider(GameObject* parent, OctreeData* octree) : Collider(parent) {
    _octree = octree;
}

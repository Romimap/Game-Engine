#include "octreecollider.h"

OctreeCollider::OctreeCollider() {

}

void OctreeCollider::Init() {
    _octreeComponent = _parent->GetComponent<OctreeComponent>();
    SetAABB();
}

void OctreeCollider::SetAABB() {
    _localAABB._min = QVector3D(0, 0, 0);
    _localAABB._max = QVector3D(_octreeComponent->getXSize(), _octreeComponent->getYSize(), _octreeComponent->getZSize());
}

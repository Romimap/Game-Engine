#include "planecollider.h"

PlaneCollider::PlaneCollider(float xSize, float zSize) : _xSize(xSize), _zSize(zSize){
    PlaneCollider::SetAABB();
}

void PlaneCollider::SetAABB() {
    _localAABB = AABB(QVector3D(-_xSize * 0.5, -0.1, -_zSize * 0.5), QVector3D(_xSize * 0.5, 0.1, _zSize * 0.5));
}

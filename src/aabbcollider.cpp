#include "aabbcollider.h"

AABBCollider::AABBCollider (float x, float y, float z) : _size(QVector3D(x * 0.5, y * 0.5, z * 0.5)) {
    AABBCollider::SetAABB();
}

void AABBCollider::SetAABB() {
    _localAABB = AABB(-_size, _size);
}

bool AABBCollider::Collision(Collider *other) {
    if (typeid (*other) == typeid (AABBCollider)) return true;
    return false;
}

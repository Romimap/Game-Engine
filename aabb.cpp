#include "aabb.h"


AABB::AABB() {}

AABB::AABB(QVector3D min, QVector3D max) {
    _min = min;
    _max = max;
}

bool AABB::intersect(AABB b) {
    return (_min.x() <= b._max.x() && _max.x() >= b._min.x()) &&
           (_min.y() <= b._max.y() && _max.y() >= b._min.y()) &&
           (_min.z() <= b._max.z() && _max.z() >= b._min.z());
}

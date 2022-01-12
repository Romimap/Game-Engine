#include "aabb.h"


AABB::AABB() {}

AABB::AABB(QVector3D min, QVector3D max) {
    _min = min;
    _max = max;
}

///Checks if AABB b intersects with this AABB
bool AABB::intersect(AABB& b) {
    return (_min.x() <= b._max.x() && _max.x() >= b._min.x()) &&
           (_min.y() <= b._max.y() && _max.y() >= b._min.y()) &&
           (_min.z() <= b._max.z() && _max.z() >= b._min.z());
}

///Checks if the ray origin, direction intersects with this AABB
float AABB::RayIntersect(QVector3D origin, QVector3D direction) {
    float tmax = (_max.x() - origin.x()) / direction.x();
    float tmin = (_min.x() - origin.x()) / direction.x();

    if (tmin > tmax) {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (_min.y() - origin.y()) / direction.y();
    float tymax = (_max.y() - origin.y()) / direction.y();

    if (tymin > tymax) {
        float tmp = tymin;
        tymin = tymax;
        tymax = tmp;
    }

    if ((tmin > tymax) || (tymin > tmax)) {
        return std::numeric_limits<float>::max();
    }

    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    float tzmin = (_min.z() - origin.z()) / direction.z();
    float tzmax = (_max.z() - origin.z()) / direction.z();

    if (tzmin > tzmax) {
        float tmp = tzmin;
        tzmin = tzmax;
        tzmax = tmp;
    }

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return std::numeric_limits<float>::max();
    }

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    if (tmin < 0 && tmax > 0) return 0;
    if (tmin > 0 && tmin < 1000) return tmin;
    if (tmax > 0 && tmax < 1000) return tmax;
    return std::numeric_limits<float>::max();
}


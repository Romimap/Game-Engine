#ifndef AABB_H
#define AABB_H

#include<QVector3D>

struct AABB {
    QVector3D _min;
    QVector3D _max;

    AABB();
    AABB(QVector3D min, QVector3D max);
    bool intersect(AABB& with);
    float RayIntersect(QVector3D origin, QVector3D direction);
};

#endif // AABB_H

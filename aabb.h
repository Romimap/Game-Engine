#ifndef AABB_H
#define AABB_H

#include<QVector3D>

struct AABB {
    QVector3D _min;
    QVector3D _max;

    AABB();
    AABB(QVector3D min, QVector3D max);
    bool intersect(AABB with);
};

#endif // AABB_H

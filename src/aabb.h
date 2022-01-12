/*
    AABB.H
    defines an AABB using two vectors
*/

#ifndef AABB_H
#define AABB_H


#include <cmath>

#include<QVector3D>


class AABB {
    /*** ATTRIBUTES ***/
public:
    QVector3D _min;
    QVector3D _max;

    /*** METHODS ***/
public:
    AABB();
    AABB(QVector3D min, QVector3D max);

    bool intersect(AABB& with);

    float RayIntersect(QVector3D origin, QVector3D direction);
};

#endif // AABB_H

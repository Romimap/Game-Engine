#ifndef OCTREECOLLIDER_H
#define OCTREECOLLIDER_H


#define EPSILON 0.0001


#include "src/collider.h"
#include "src/gameobject.h"

#include "src/components/worldgeneratorcomponent.h"
#include "src/components/terrain/octreecomponent.h"


class OctreeCollider : public Collider {
    /*** ATTRIBUTES ***/
private:
    OctreeComponent* _octreeComponent;

    /*** METHODS ***/
public:
    OctreeCollider();

    void Init() override;
    void SetAABB() override;
    RayCastHit RayCast(QVector3D origin, QVector3D destination) override;

private:
    bool BoxIntersect(QVector3D origin, QVector3D direction, QVector3D min, QVector3D max, float &tmin, float &tmax);
    RayCastHit GridTreeIntersect (QVector3D O, QVector3D D, QVector3D gridPos, QVector3D offset, QVector3D gridSize, int layer);
};

#endif // OCTREECOLLIDER_H

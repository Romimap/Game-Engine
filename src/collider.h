#ifndef COLLIDER_H
#define COLLIDER_H


#include <QMatrix4x4>

#include "src/aabb.h"


class GameObject;
class PlaneCollider;
class CubeCollider;


class Collider {
    /*** ATTRIBUTES ***/
public:
    /// The AABB that contains that collider, but in local space, should not be used as is.
    /// It is used by gameobjects to quickly approximate the global space AABB
    AABB _localAABB;
    GameObject* _parent;

    /*** METHODS ***/
public:
    Collider(){}
    virtual ~Collider(){}

    virtual void SetAABB(){}

    virtual void Init(){}
    virtual GameObject* GetParent() {return _parent;}

    virtual bool Collision(Collider* other) {
        qDebug("COLLISION VIRTUAL");
        return false;
    }
};

#endif // COLLIDER_H

#ifndef COLLIDER_H
#define COLLIDER_H

#include "aabb.h"
#include <QMatrix4x4>

class GameObject;
class PlaneCollider;
class CubeCollider;

class Collider {
public:
    /// The AABB that contains that collider, but in local space, should not be used as is.
    /// It is used by gameobjects to quickly approximate the global space AABB
    AABB _localAABB;
    GameObject* _parent;

public:
    Collider() {
    }
    virtual ~Collider() {

    }

public:
    virtual void SetAABB () {}
    virtual GameObject* GetParent() {return _parent;}
    virtual bool Collision(Collider* other) {
        qDebug("COLLISION VIRTUAL");
        return false;
    }
};

#endif // COLLIDER_H

#ifndef COLLIDER_H
#define COLLIDER_H

#include "aabb.h"
#include <QMatrix4x4>

class GameObject;
class PlaneCollider;
class CubeCollider;

struct CollisionData {
    bool collision;
    QVector3D position;
    QVector3D normal;
    GameObject* a;
    GameObject* b;
};

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
    virtual CollisionData Collision(Collider* other) {
        qDebug("COLLISION VIRTUAL");
        CollisionData collisionData;
        collisionData.collision = false;
        return collisionData;
    }


};

#endif // COLLIDER_H

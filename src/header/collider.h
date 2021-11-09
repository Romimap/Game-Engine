#ifndef COLLIDER_H
#define COLLIDER_H

#include"src/header/aabb.h"

class GameObject;


struct CollisionData {
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
    Collider(GameObject* parent) {
        _parent = parent;
    }
    virtual ~Collider() {

    }

public:
    virtual void SetAABB () {}
};

#endif // COLLIDER_H

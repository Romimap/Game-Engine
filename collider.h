#ifndef COLLIDER_H
#define COLLIDER_H

#include"aabb.h"

class Collider {
public:
    AABB _AABB;

public:
    Collider() {
        _AABB = SetAABB();
    }

protected:
    virtual AABB SetAABB ();
};

#endif // COLLIDER_H

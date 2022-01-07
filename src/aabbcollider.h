#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H


#include "src/collider.h"


class AABBCollider : public Collider {
    /*** ATTRIBUTES ***/
private:
    QVector3D _size;

    /*** METHODS ***/
public:
    AABBCollider(float x, float y, float z);

    bool Collision(Collider* other) override;

    void SetAABB() override;
};

#endif // AABBCOLLIDER_H

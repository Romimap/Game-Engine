#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H

#include "collider.h"

class AABBCollider : public Collider {
private:
    QVector3D _size;
public:
    AABBCollider(float x, float y, float z);

    bool Collision(Collider* other) override;

    void SetAABB() override;

};

#endif // AABBCOLLIDER_H

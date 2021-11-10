#ifndef PLANECOLLIDER_H
#define PLANECOLLIDER_H

#include "collider.h"

class PlaneCollider : public Collider {
private:
    float _xSize = 10;
    float _zSize = 10;
public:
    PlaneCollider(float xSize = 10, float zSize = 10);

public:
    void SetAABB() override;
};

#endif // PLANECOLLIDER_H

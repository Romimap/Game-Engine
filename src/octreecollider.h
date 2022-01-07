#ifndef OCTREECOLLIDER_H
#define OCTREECOLLIDER_H

#include "gameobject.h"
#include "collider.h"
#include "components/terrain/octreecomponent.h"

class OctreeCollider : public Collider {

private:
    OctreeComponent* _octreeComponent;

public:
    OctreeCollider();
    void Init() override;
    void SetAABB() override;
};

#endif // OCTREECOLLIDER_H

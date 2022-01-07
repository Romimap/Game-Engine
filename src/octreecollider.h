#ifndef OCTREECOLLIDER_H
#define OCTREECOLLIDER_H


#include "src/collider.h"
#include "src/gameobject.h"

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
};

#endif // OCTREECOLLIDER_H

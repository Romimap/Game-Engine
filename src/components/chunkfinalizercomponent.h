#ifndef CHUNKFINALIZERCOMPONENT_H
#define CHUNKFINALIZERCOMPONENT_H

#include <chrono>
#include <iostream>

#include "src/component.h"

#include "src/components/octreerenderercomponent.h"


class ChunkFinalizerComponent : public Component {
    /*** METHODS ***/
public:
    ChunkFinalizerComponent(GameObject* parent);

    void Start() override;
};

#endif // CHUNKFINALIZERCOMPONENT_H

#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include "src/header/component.h"

class RotatorComponent : public Component
{
public:
    RotatorComponent(GameObject* parent);

public:
    void Update(float delta) override;
};

#endif // ROTATORCOMPONENT_H

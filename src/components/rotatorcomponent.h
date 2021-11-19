#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include "src/component.h"

class RotatorComponent : public Component
{
private:
    float _xspeed = 0;
    float _yspeed = 0;
    float _zspeed = 0;
public:
    RotatorComponent(GameObject* parent, float xspeed, float yspeed, float zspeed);

public:
    void Update(float delta) override;
};

#endif // ROTATORCOMPONENT_H

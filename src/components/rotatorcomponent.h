#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H


#include "src/component.h"
#include "src/engine.h"


class RotatorComponent : public Component {
    /*** ATTRIBUTES ***/
private:
    float _xspeed = 0;
    float _yspeed = 0;
    float _zspeed = 0;

    /*** METHODS ***/
public:
    RotatorComponent(GameObject* parent, float xspeed, float yspeed, float zspeed);

    void Update(float delta) override;
};

#endif // ROTATORCOMPONENT_H

#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H


#include <QCursor>
#include <QQuaternion>
#include <QVector3D>
#include <Qt>

#include "src/component.h"
#include "src/engine.h"
#include "src/inputmanager.h"

#include "src/components/rigidbodycomponent.h"


class PlayerControllerComponent : public Component {
    /*** ATTRIBUTES ***/
private:
    float _speed;
    float _acceleration;

    float _azimuth = 0;
    float _elevation = 0;

    QVector3D _momentum;

    RigidBodyComponent* _cubeRb;

    /*** METHODS ***/
public:
    PlayerControllerComponent(float speed, float acceleration, RigidBodyComponent* cubeRb, GameObject* parent);
    virtual ~PlayerControllerComponent();

    void Start() override;
    void Update(float delta) override;
};

#endif // PLAYERCONTROLLERCOMPONENT_H

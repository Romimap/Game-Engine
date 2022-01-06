#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

#include "src/component.h"
#include <Qt>
#include <QQuaternion>
#include <QVector3D>
#include "src/inputmanager.h"
#include <QCursor>
#include "rigidbodycomponent.h"

class PlayerControllerComponent : public Component {
private:
    float _speed;
    float _acceleration;

    float _azimuth = 0;
    float _elevation = 0;

    QVector3D _momentum;

    RigidBodyComponent* _cubeRb;

public:
    PlayerControllerComponent(float speed, float acceleration, RigidBodyComponent* cubeRb, GameObject* parent);
    virtual ~PlayerControllerComponent();

    void Start() override;
    void Update(float delta) override;
};

#endif // PLAYERCONTROLLERCOMPONENT_H

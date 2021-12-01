#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

#include "src/component.h"
#include <Qt>
#include <QQuaternion>
#include <QVector3D>
#include "src/inputmanager.h"
#include <QCursor>
#include "rigidbodycomponent.h"

class PlayerControllerComponent : public Component
{
private:
    float _speed;
    float _acceleration;
    QVector3D _momentum;
    RigidBodyComponent* _cubeRb;

    float _azimuth = 0;
    float _elevation = 0;
public:
    PlayerControllerComponent(float speed, float acceleration, RigidBodyComponent* cubeRb, GameObject* parent);

    void Start() override {
        QQuaternion rotation = GetParent()->GetTransform()->GetRotation();
        _elevation = rotation.toEulerAngles().x();
        _azimuth = rotation.toEulerAngles().y();
    }

    void Update(float delta) override {
        //Mouse Capture
        if (InputManager::Key('P')) InputManager::SetCaptureMouse(true);
        if (InputManager::Key('M')) InputManager::SetCaptureMouse(false);

        //PushPull Cube
        if (InputManager::Key('T') && _cubeRb != nullptr) _cubeRb->_momentum += GetParent()->GetTransform()->Forward();
        if (InputManager::Key('G') && _cubeRb != nullptr) _cubeRb->_momentum -= GetParent()->GetTransform()->Forward();

        //Movement & Orientation
        float x = InputManager::Key('D') - InputManager::Key('Q');
        float y = InputManager::Key('F') - InputManager::Key('R');
        float z = InputManager::Key('S') - InputManager::Key('Z');
        QVector3D desiredMovement = z * GetParent()->GetTransform()->Forward() + x * GetParent()->GetTransform()->Right() + y * GetParent()->GetTransform()->Up();
        desiredMovement *= _speed;
        _momentum = desiredMovement * _acceleration + _momentum * (1 - _acceleration);

        _azimuth += InputManager::MouseDX() * 0.3;
        _elevation += InputManager::MouseDY() * 0.3;

        if (_elevation > 90) _elevation =  90;
        if (_elevation <-90) _elevation = -90;
        if (_azimuth <-180) _azimuth += 360;
        if (_azimuth > 180) _azimuth -= 360;


        QQuaternion elevationQuat = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), _elevation);
        QQuaternion azimuthQuat = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), _azimuth);;

        GetParent()->GetTransform()->Translate(_momentum * delta);
        GetParent()->GetTransform()->SetRotation(azimuthQuat * elevationQuat);

        qDebug((std::to_string(GetParent()->GetTransform()->GetPosition().x()) + " " + std::to_string(GetParent()->GetTransform()->GetPosition().y()) + " " + std::to_string(GetParent()->GetTransform()->GetPosition().z())).c_str());
    }
};

#endif // PLAYERCONTROLLERCOMPONENT_H

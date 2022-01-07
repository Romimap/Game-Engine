#include "playercontrollercomponent.h"
#include "src/engine.h"

PlayerControllerComponent::PlayerControllerComponent(float speed, float acceleration, RigidBodyComponent* cubeRb, GameObject* parent) : Component(parent), _cubeRb(cubeRb), _speed(speed), _acceleration(acceleration) {
    this->_name = "PlayerControllerComponent";
}

PlayerControllerComponent::~PlayerControllerComponent() {
    if (_cubeRb != nullptr)
        delete _cubeRb;
}

void PlayerControllerComponent::Start() {
    QQuaternion rotation = GetParent()->GetTransform()->GetRotation();
    _elevation = rotation.toEulerAngles().x();
    _azimuth = rotation.toEulerAngles().y();
}

void PlayerControllerComponent::Update(float delta) {
    //Mouse Capture
    if (InputManager::Key('P')) InputManager::SetCaptureMouse(true);
    if (InputManager::Key('M')) InputManager::SetCaptureMouse(false);

    //Raycast
    if (InputManager::Key('E')) {
        Engine::Singleton->RayCast(GetParent()->GetTransform()->GetGlobalPosition(), -GetParent()->GetTransform()->Forward());
    }
    //Movement & Orientation
    float x = InputManager::Key('D') - InputManager::Key('Q');
    float y = InputManager::Key('R') - InputManager::Key('F');
    float z = InputManager::Key('S') - InputManager::Key('Z');
    QVector3D desiredMovement = z * GetParent()->GetTransform()->Forward() + x * GetParent()->GetTransform()->Left() + y * GetParent()->GetTransform()->Up();
    desiredMovement *= _speed;
    _momentum = desiredMovement * _acceleration + _momentum * (1 - _acceleration);

    _azimuth -= InputManager::MouseDX() * 0.3;
    _elevation -= InputManager::MouseDY() * 0.3;

    if (_elevation > 90) _elevation =  90;
    if (_elevation <-90) _elevation = -90;
    if (_azimuth <-180) _azimuth += 360;
    if (_azimuth > 180) _azimuth -= 360;


    QQuaternion elevationQuat = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), _elevation);
    QQuaternion azimuthQuat = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), _azimuth);;

    GetParent()->GetTransform()->Translate(_momentum * delta);
    GetParent()->GetTransform()->SetRotation(azimuthQuat * elevationQuat);

    //qDebug((std::to_string(GetParent()->GetTransform()->GetPosition().x())
    //+ " " + std::to_string(GetParent()->GetTransform()->GetPosition().y())
    //+ " " + std::to_string(GetParent()->GetTransform()->GetPosition().z())
    //+ " (playercontrollercomponent.cpp)").c_str());
}

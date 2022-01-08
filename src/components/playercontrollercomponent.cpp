#include "playercontrollercomponent.h"
#include "src/components/terrain/octreecomponent.h"

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

    //Dig
    if (InputManager::Key('E')) {
        RayCastHit hit =  Engine::Singleton->RayCast(GetParent()->GetTransform()->GetGlobalPosition(), -GetParent()->GetTransform()->Forward());
        if (hit._gameobject != nullptr) {
            OctreeComponent* octreeComponent = hit._gameobject->GetComponent<OctreeComponent>();
            if (octreeComponent != nullptr) {
                QVector3D coord = GetParent()->GetTransform()->GetGlobalPosition() + (-GetParent()->GetTransform()->Forward() * (hit._distance + 0.1));
                coord = coord - hit._gameobject->GetTransform()->GetPosition();

                for (int x = -_editRadius; x <= _editRadius; x++) {
                    for (int y = -_editRadius; y <= _editRadius; y++) {
                        for (int z = -_editRadius; z <= _editRadius; z++) {
                            QVector3D r(x, y, z);
                            if (r.length() <= _editRadius + 0.1) {
                                octreeComponent->setVoxelType(coord.x() + x, coord.y() + y, coord.z() + z, MaterialId::AIR);
                            }
                        }
                    }
                }
            }
        }
    }

    //Fill
    if (InputManager::Key('A')) {
        RayCastHit hit =  Engine::Singleton->RayCast(GetParent()->GetTransform()->GetGlobalPosition(), -GetParent()->GetTransform()->Forward());
        if (hit._gameobject != nullptr) {
            OctreeComponent* octreeComponent = hit._gameobject->GetComponent<OctreeComponent>();
            if (octreeComponent != nullptr) {
                QVector3D coord = GetParent()->GetTransform()->GetGlobalPosition() + (-GetParent()->GetTransform()->Forward() * (hit._distance + 0.1));
                coord = coord - hit._gameobject->GetTransform()->GetPosition();

                for (int x = -_editRadius; x <= _editRadius; x++) {
                    for (int y = -_editRadius; y <= _editRadius; y++) {
                        for (int z = -_editRadius; z <= _editRadius; z++) {
                            QVector3D r(x, y, z);
                            if (r.length() <= _editRadius + 0.1) {
                                octreeComponent->setVoxelType(coord.x() + x, coord.y() + y, coord.z() + z, MaterialId::STONE);
                            }
                        }
                    }
                }
            }
        }
    }

    //Gravity
    if (_onGround <= 0) {
        _momentum += _gravity;
    }
    //Ground
    _onGround -= delta;
    if (_momentum.y() <= 0) {
        RayCastHit hit =  Engine::Singleton->RayCast(GetParent()->GetTransform()->GetGlobalPosition(), QVector3D(0.01, -1, 0.01));
        if (hit._gameobject != nullptr && hit._distance <= _playerHeight + 1.5) { //We are going down to the ground
            _momentum.setY(0);
            _onGround = _coyoteTime;
            GetParent()->GetTransform()->Translate(QVector3D(0, (_playerHeight - hit._distance) / 8, 0));
        }
    }

    //Movement & Orientation
    float x = InputManager::Key('D') - InputManager::Key('Q');
    float y = InputManager::Key('R') - InputManager::Key('F');
    float z = InputManager::Key('S') - InputManager::Key('Z');
    QVector3D forward = GetParent()->GetTransform()->Forward();
    forward.setY(0);
    forward.normalize();
    QVector3D desiredMovement = z * forward + x * GetParent()->GetTransform()->Left() + y * GetParent()->GetTransform()->Up();
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

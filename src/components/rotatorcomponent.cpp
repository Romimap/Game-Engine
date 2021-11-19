#include "rotatorcomponent.h"
#include "src/engine.h"

RotatorComponent::RotatorComponent(GameObject* parent, float xspeed, float yspeed, float zspeed) : Component(parent), _xspeed(xspeed), _yspeed(yspeed), _zspeed(zspeed) {

}

void RotatorComponent::Update(float delta) {
    GetParent()->GetTransform()->RotateAround(_xspeed * delta, QVector3D(1, 0, 0));
    GetParent()->GetTransform()->RotateAround(_yspeed * delta, QVector3D(0, 1, 0));
    GetParent()->GetTransform()->RotateAround(_zspeed * delta, QVector3D(0, 0, 1));

    qDebug((std::to_string(Engine::Singleton->tick) + " " + GetParent()->NAME).c_str());
}

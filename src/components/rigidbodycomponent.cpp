//TODO: do things with global positions, here we use local transform, this would break as soon as globalTransform != localTransform

#include "rigidbodycomponent.h"

QVector3D RigidBodyComponent::gravity(0, -9.8, 0);
float RigidBodyComponent::bounceTreshold = 0.001;
float RigidBodyComponent::frictionTreshold = 0.02;


RigidBodyComponent::RigidBodyComponent(float weight, float drag, float bounciness, float friction, GameObject* parent) : Component(parent), _weight(weight), _drag(1 - drag), _bounciness(bounciness), _friction(1 - friction) {

}

RigidBodyComponent::~RigidBodyComponent() {
    if (_previousTransform != nullptr)
        delete _previousTransform;
}

void RigidBodyComponent::FixedUpdate(float delta) {
    if (_previousTransform != nullptr)
        delete _previousTransform;
    _previousTransform = new Transform(GetParent()->GetTransform(), GetParent());

    GetParent()->GetTransform()->Translate(_momentum * delta);

    _momentum += gravity * delta;
    _momentum *= _drag;
}

void RigidBodyComponent::Collision(Collider *other) {
    Collider *mine = GetParent()->GetCollider();

    //AABB - AABB
    if (typeid(*mine) == typeid (AABBCollider) && typeid (*other) == typeid (AABBCollider)) {
        ResolveAABBAABB((AABBCollider*)mine, (AABBCollider*)other);
    }
}

void RigidBodyComponent::ResolveAABBAABB(AABBCollider *mine, AABBCollider *other) {
    qDebug("RESOLVE AABB AABB");
    //Store velocity
    QVector3D velocity = _previousTransform->GetPosition() - GetParent()->GetTransform()->GetPosition();

    //Revert to previous position
    GetParent()->GetTransform()->SetPosition(_previousTransform->GetPosition());
    GetParent()->RefreshAABB();

    //Calculate the deltaposition of our aabbs
    QVector3D deltaPosition;

    AABB* myAABB = mine->GetParent()->GetPersonalGlobalAABB();
    AABB* otherAABB = other->GetParent()->GetPersonalGlobalAABB();

    if (myAABB->_min.x() < otherAABB->_min.x()) {
        deltaPosition.setX(otherAABB->_min.x() - (myAABB->_min.x() + (myAABB->_max.x() - myAABB->_min.x())));
    } else {
        deltaPosition.setX(myAABB->_min.x() - (otherAABB->_min.x() + (otherAABB->_max.x() - otherAABB->_min.x())));
    }

    if (myAABB->_min.y() < otherAABB->_min.y()) {
        deltaPosition.setY(otherAABB->_min.y() - (myAABB->_min.y() + (myAABB->_max.y() - myAABB->_min.y())));
    } else {
        deltaPosition.setY(myAABB->_min.y() - (otherAABB->_min.y() + (otherAABB->_max.y() - otherAABB->_min.y())));
    }

    if (myAABB->_min.z() < otherAABB->_min.z()) {
        deltaPosition.setZ(otherAABB->_min.z() - (myAABB->_min.z() + (myAABB->_max.z() - myAABB->_min.z())));
    } else {
        deltaPosition.setZ(myAABB->_min.z() - (otherAABB->_min.z() + (otherAABB->_max.z() - otherAABB->_min.z())));
    }

    //Time to collide
    float xTimeToCollide =  velocity.x() != 0 ? abs(deltaPosition.x() / velocity.x()) : 0;
    float yTimeToCollide =  velocity.y() != 0 ? abs(deltaPosition.y() / velocity.y()) : 0;
    float zTimeToCollide =  velocity.z() != 0 ? abs(deltaPosition.z() / velocity.z()) : 0;

    float timeToCollide = std::min(xTimeToCollide, std::min(yTimeToCollide, zTimeToCollide));

    //
    GetParent()->GetTransform()->Translate(velocity * timeToCollide);
    GetParent()->RefreshAABB();

    //Bounce
    if (xTimeToCollide < yTimeToCollide && xTimeToCollide < zTimeToCollide) {
        float bounce = -_momentum.x() * _bounciness;
        if (bounce < bounceTreshold) bounce = 0;
        _momentum.setX(bounce);
    }
    if (yTimeToCollide < xTimeToCollide && yTimeToCollide < zTimeToCollide) {
        float bounce = -_momentum.y() * _bounciness;
        if (bounce < bounceTreshold) bounce = 0;
        _momentum.setY(bounce);
    }
    if (zTimeToCollide < yTimeToCollide && zTimeToCollide < xTimeToCollide) {
        float bounce = -_momentum.z() * _bounciness;
        if (bounce < bounceTreshold) bounce = 0;
        _momentum.setZ(bounce);
    }

    //Friction
    _momentum *= _friction;
    if (_momentum.x() < frictionTreshold) _momentum.setX(0);
    if (_momentum.y() < frictionTreshold) _momentum.setY(0);
    if (_momentum.z() < frictionTreshold) _momentum.setZ(0);
}


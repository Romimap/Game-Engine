#include "rigidbodycomponent.h"

QVector3D RigidBodyComponent::gravity(0, -9.8, 0);


RigidBodyComponent::RigidBodyComponent(float weight, GameObject* parent) : Component(parent), _weight(weight) {

}

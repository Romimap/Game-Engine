#include "rigidbodycomponent.h"

QVector3D RigidBodyComponent::gravity(0, -9.8, 0);
float RigidBodyComponent::speedTreshold = 5;

RigidBodyComponent::RigidBodyComponent(float weight, GameObject* parent) : Component(parent), _weight(weight) {

}

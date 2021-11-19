#include "playercontrollercomponent.h"

PlayerControllerComponent::PlayerControllerComponent(float speed, float acceleration, RigidBodyComponent* cubeRb, GameObject* parent) : Component(parent), _cubeRb(cubeRb), _speed(speed), _acceleration(acceleration) {

}

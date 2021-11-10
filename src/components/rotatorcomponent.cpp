#include "rotatorcomponent.h"

RotatorComponent::RotatorComponent(GameObject* parent) : Component(parent) {

}

void RotatorComponent::Update(float delta) {
    GetParent()->GetTransform()->Rotate(0, 100 * delta, 0);
}

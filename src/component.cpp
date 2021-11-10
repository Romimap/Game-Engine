#include "component.h"

Component::Component(GameObject* parent) : _parent(parent) {
    _parent->AddComponent(this);
}

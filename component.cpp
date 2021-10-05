#include "component.h"
class Component;


Component::Component() : _parent(nullptr) {}

void Component::SetParent(GameObject *parent) {
    _parent = parent;
}

void Component::Start() {

}

void Component::Update(float delta) {

}

void Component::Enable() {

}

void Component::Disable() {

}

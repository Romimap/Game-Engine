#include "gameobject.h"

GameObject::GameObject() {

}

void GameObject::Enable () {
    if (_enabled) return;
    _enabled = true;
    for (Component* c : _componentList) {
       c->Enable();
    }
}

void GameObject::Disable (){
    if (!_enabled) return;
    _enabled = false;
    for (Component* c : _componentList) {
       c->Disable();
    }
}

void GameObject::Start (){
    if (_started) return;
    _started = true;
    for (Component* c : _componentList) {
       c->Enable();
    }
}

void GameObject::Update (float delta){
    for (Component* c : _componentList) {
       c->Update(delta);
    }
}

void GameObject::AddComponent (Component& component){
    _componentList.push_back(&component);
    component.SetParent(this);
    if (_enabled)
        component.Enable();
    else
        component.Disable();
}

template<typename T> T& GameObject::GetComponent (){
    for (Component* c : _componentList) {
       if (typeid (&c) == typeid (T)) {
           return &c;
       }
    }
}

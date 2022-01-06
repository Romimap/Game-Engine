#include "component.h"

Component::Component(GameObject* parent) : _parent(parent) {
    this->_name = "Component";
    _parent->AddComponent(this);
}

Component::~Component() {
//    std::cout << "# Deleting '" << _name << "' Component..." << std::endl;
}

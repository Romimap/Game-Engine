#ifndef COMPONENT_H
#define COMPONENT_H

#include "gameobject.h"
#include "collider.h"

#include <iostream>

class GameObject;
class Component {
private:
    GameObject* _parent;
public:
    std::string _name;

    Component(GameObject* parent);
    virtual ~Component();

    virtual void Start(){}
    virtual void Update(float delta){}
    virtual void FixedUpdate(float delta){}
    virtual void Collision(Collider* other){}
    virtual void Draw(){}

protected:
    GameObject* GetParent() {return _parent;}
};

#endif // COMPONENT_H

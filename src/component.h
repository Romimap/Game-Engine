#ifndef COMPONENT_H
#define COMPONENT_H


#include <iostream>

#include "src/gameobject.h"
#include "src/collider.h"


class GameObject;


class Component {
    /*** ATTRIBUTES ***/
private:
    GameObject* _parent;

public:
    std::string _name;

    /*** METHODS ***/
public:
    Component(GameObject* parent);
    virtual ~Component(){}

    virtual void Start(){}
    virtual void Update(float delta){}
    virtual void FixedUpdate(float delta){}
    virtual void Collision(Collider* other){}
    virtual void Draw(){}

protected:
    GameObject* GetParent() {return _parent;}
};

#endif // COMPONENT_H

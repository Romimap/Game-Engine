#ifndef COMPONENT_H
#define COMPONENT_H

#include "gameobject.h"
#include "collider.h"

class GameObject;
class Component {
private:
    GameObject* _parent;
public:
    Component(GameObject* parent);

    virtual void Start(){}
    virtual void Update(float delta){}
    virtual void FixedUpdate(float delta){}
    virtual void Collision(Collider* other){}

protected:
    GameObject* GetParent() {return _parent;}
};

#endif // COMPONENT_H

#ifndef COMPONENT_H
#define COMPONENT_H

#include "gameobject.h"

class GameObject;
class Component {
private:
    GameObject* _parent;
public:
    Component(GameObject* parent);

    virtual void Start();
    virtual void Update(float delta);

protected:
    GameObject* GetParent() {return _parent;}
};

#endif // COMPONENT_H

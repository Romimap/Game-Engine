#ifndef COMPONENT_H
#define COMPONENT_H

#include "gameobject.h"

class GameObject;
class Component {
    //MEMBERS
private:
    GameObject* _parent;

    //CONSTRUCTORS
public:
    Component();

    //GETTERS SETTERS
public:
    void SetParent (GameObject* parent);

    //METHODS
public:
    virtual void Update (float);
    virtual void Enable ();
    virtual void Disable ();
    virtual void Start ();

};

#endif // COMPONENT_H

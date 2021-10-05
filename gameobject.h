#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "component.h"
#include "transform.h"
#include <qquaternion.h>
#include <qvector3d.h>
#include <vector>



class Component;
class GameObject {
    //MEMBERS
private:
    bool _enabled = true;
    bool _started = false;
    Transform _localTransform;
    std::vector<Component*> _componentList;
    GameObject* _parent;
    std::vector<GameObject*> _children;

    //CONSTRUCTORS
public:
    GameObject();


    //GETTERS SETTERS
public:
    bool Enabled () {return _enabled;}


    //METHODS
public:
    void Enable ();
    void Disable ();
    void Start ();
    void Update (float delta);
    void AddComponent (Component& component);
    template<typename T> T& GetComponent ();
};


#endif // GAMEOBJECT_H

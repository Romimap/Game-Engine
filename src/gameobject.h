#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "transform.h"
#include "component.h"
#include "aabb.h"
#include "collider.h"

#include <limits>

#include <qquaternion.h>
#include <qvector3d.h>
#include <vector>


class Collider;
class Transform;
class Engine;
struct RenderData;
class Component;

class GameObject {
    //MEMBERS
protected:
    bool _enabled = true;
    bool _started = false;

    GameObject* _parent = nullptr;

    Transform* _transform = nullptr;
    Collider* _collider = nullptr;

    // The AABB that contains the collider of that gameobject AND all children, in global space
    AABB* _globalAABB = nullptr;
    // The AABB that contains the collider of that gameobject, in global space
    AABB* _personalGlobalAABB = nullptr;

    std::vector<GameObject*>* _children = nullptr;
    std::vector<Component*>* _components = nullptr;

public:
    static GameObject* Root;

    std::string NAME = "";

    //CONSTRUCTORS
public:
    GameObject(GameObject* parent = nullptr);
    ~GameObject();

    //METHODS
    bool Enabled();

    void AddComponent (Component* component);

    void Draw();

    //GETTERS SETTERS

    GameObject* GetParent();

    Transform* GetTransform();
    void SetTransform(Transform* t);

    Collider* GetCollider() {return _collider;}
    void SetCollider (Collider* collider);

    AABB* GetGlobalAABB() {return _globalAABB;}
    AABB* GetPersonalGlobalAABB() {return _personalGlobalAABB;}

    std::vector<GameObject*>* GetChildren();
    template<typename T> Component* GetComponent();

    //METHODS
public:
    void Enable();
    void Disable();
    void Start();
    void Update(float delta);
    void FixedUpdate(float delta);
    void Collisions(GameObject* current);
    void RefreshAABB();

};

#endif // GAMEOBJECT_H

            #ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "transform.h"
#include "component.h"
#include "aabb.h"
#include "collider.h"

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
    ///The AABB that contains the collider of that gameobject AND all children, in global space
    AABB* _globalAABB = nullptr;
    ///The AABB that contains the collider of that gameobject, in global space
    AABB* _personalGlobalAABB = nullptr;

    Collider* _collider = nullptr;
    bool _enabled = true;
    bool _started = false;
    GameObject* _parent = nullptr;
    std::vector<GameObject*>* _children = nullptr;
    std::vector<Component*>* _components = nullptr;
    Transform* _transform = nullptr;
    RenderData* _renderData = nullptr;

public:
    static GameObject* Root;

    //CONSTRUCTORS
public:
    std::string NAME = "";
    GameObject(GameObject* parent = nullptr);
    ~GameObject();

    //GETTERS SETTERS
public:
    bool Enabled ();
    Transform* GetTransform ();
    void SetTransform (Transform* t);
    GameObject* GetParent ();
    std::vector<GameObject*>* GetChildren();
    RenderData* GetRenderData();
    void SetRenderData(RenderData* renderData);
    void AddComponent (Component* component);
    void SetCollider (Collider* collider);
    Collider* GetCollider() {return _collider;}
    AABB* GetGlobalAABB() {return _globalAABB;}
    AABB* GetPersonalGlobalAABB() {return _personalGlobalAABB;}
    template<typename  T> Component* GetComponent ();

    //METHODS
public:
    void Enable ();
    void Disable ();
    void Start ();
    void Update (float delta);
    void FixedUpdate (float delta);
    void Collisions(GameObject* current);
    void RefreshAABB();

};

#endif // GAMEOBJECT_H

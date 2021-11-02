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
private:
    AABB* _AABB = nullptr;
    Collider* _collider;
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
    GameObject* GetParent ();
    std::vector<GameObject*>* GetChildren();
    RenderData* GetRenderData();
    void SetRenderData(RenderData* renderData);
    void AddComponent (Component* component);

    //METHODS
public:
    void Enable ();
    void Disable ();
    void Start ();
    void Update (float delta);
    AABB* RefreshAABB();

};

#endif // GAMEOBJECT_H

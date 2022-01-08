#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <QQuaternion>
#include <QVector3D>

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "src/aabb.h"
#include "src/collider.h"
#include "src/component.h"
#include "src/transform.h"
#include "src/structs.h"

class Collider;
class Transform;
class Engine;
struct RenderData;
class Component;


class GameObject {
    /*** ATTRIBUTES ***/
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

    std::vector<GameObject*> _children;
    std::vector<Component*> _components;

public:
    static GameObject* Root;

    std::string _name = "";

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    GameObject(std::string name, GameObject* parent = nullptr);
    ~GameObject();

    /** METHODS **/

    /** GETTERS SETTERS **/
    bool Enabled();

    GameObject* GetParent();

    Transform* GetTransform();
    void SetTransform(Transform* t);

    Collider* GetCollider() {return _collider;}
    void SetCollider (Collider* collider);

    AABB* GetGlobalAABB() {return _globalAABB;}
    AABB* GetPersonalGlobalAABB() {return _personalGlobalAABB;}

    std::vector<GameObject*> GetChildren();

    /** GENERIC METHODS **/
    void Enable();
    void Disable();
    void Start();
    void Update(float delta);
    void FixedUpdate(float delta);
    void Draw();

    void AddComponent(Component* component);

    void Collisions(GameObject* current);
    RayCastHit AABBRayCollision(QVector3D origin, QVector3D direction);

    void RefreshAABB();

    /** TEMPLATE METHODS **/
    template <typename T> T* GetComponent() {
        for (Component* c : _components) {
            if (T* tc = dynamic_cast<T*>(c)) return tc;
        }
        std::cerr << "GameObject::GetComponent<>() has not found the searched component" << std::endl;
        return nullptr;
    }
};

#endif // GAMEOBJECT_H

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "transform.h"
#include "component.h"
#include "aabb.h"
#include "collider.h"

#include <iostream>
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
    bool _fixedAABB = false;

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

    //CONSTRUCTORS
public:
    GameObject(std::string name, GameObject* parent = nullptr);
    ~GameObject();

    /** METHODS **/
    void AddComponent(Component* component);

    void Draw();

    /** GETTERS SETTERS **/
    bool Enabled();

    void SetFixedAABB(QVector3D min, QVector3D max);

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
    void Collisions(GameObject* current);
    void RefreshAABB();

    /** METHODS USING TEMPLATES **/
    template <typename T> T* GetComponent() {
        for (Component* c : _components) {
            if (typeid (*c) == typeid (T)) return (T*)c;
        }
        return nullptr;
    }

    template <typename T> T* GetDerivedComponent() {
        for (Component* c : _components) {
            if (T* tc = dynamic_cast<T*>(c)) return tc;
        }
        std::cout << "null !" << std::endl;
        return nullptr;
    }

};

#endif // GAMEOBJECT_H

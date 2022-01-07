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

struct RayCastHit {
    float _distance;
    QVector3D _position;
    GameObject* _gameobject;

    RayCastHit() : _distance(-1), _position(QVector3D()), _gameobject(nullptr) {

    }
};

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
    RayCastHit AABBRayCollision(QVector3D origin, QVector3D direction);

    void RefreshAABB();

    template <typename T> T* GetComponent() {
        for (Component* c : _components) {
            if (T* tc = dynamic_cast<T*>(c)) return tc;
        }
        std::cout << "null !" << std::endl;
        return nullptr;
    }

};

#endif // GAMEOBJECT_H

#include "gameobject.h"
#include <math.h>

GameObject* GameObject::Root = nullptr;

GameObject::GameObject(GameObject* parent) : _parent(parent) {
    if (parent == nullptr) {
        Root = this;
    }

    _transform = new Transform(this);
    _children = new std::vector<GameObject*>();
    _components = new std::vector<Component*>();

    if (parent != nullptr) {
        parent->_children->push_back(this);
    }
}

GameObject::~GameObject() {
    //TODO : Free the memory and do something about the orphans
}

//GETTERS SETTERS
bool GameObject::Enabled () {
    return _enabled;
}
Transform* GameObject::GetTransform () {
    return _transform;
}
GameObject* GameObject::GetParent () {
    return _parent;
}

std::vector<GameObject*>* GameObject::GetChildren() {
    return _children;
}

void GameObject::SetRenderData(RenderData *renderData) {
    _renderData = renderData;
}

void GameObject::SetCollider(Collider *collider) {
    if (_collider != nullptr) delete _collider;
    _collider = collider;
    _collider->_parent = this;
}

//METHODS
void GameObject::Enable () {//TODO
}
void GameObject::Disable () {//TODO
}

void GameObject::Start () {
    for (GameObject* child : *_children) {
        child->Start();
    }
    for (Component* component : *_components) {
        component->Start();
    }
    _started = true;
}

void GameObject::Update (float delta) {
    for (GameObject* child : *_children) {
        child->Update(delta);
    }
    for (Component* component : *_components) {
        component->Update(delta);
    }
}

void GameObject::Collisions(GameObject* current) {
    if (!current->Enabled() || current->_globalAABB == nullptr) //We wont explore this branch if the branch is disabled or if there is nothing to collide with
        return;

    if (_personalGlobalAABB->intersect(*current->_globalAABB)) { //We are "Colliding" with that branch, check current and its children !
        if (current != this && current->_personalGlobalAABB != nullptr && _personalGlobalAABB->intersect(*current->_personalGlobalAABB)) { //Current collision
            //TODO, do a collision calculation, then send a collision signal containing a CollisionData...
        }
        for (GameObject* child : *current->GetChildren()) { //Possible children collision
            Collisions(child);
        }
    }
}

RenderData* GameObject::GetRenderData() {
    return _renderData;
}

void GameObject::AddComponent(Component *component) {
    _components->push_back(component);
}

void GameObject::RefreshAABB() {
    QVector3D min( 100000000,  100000000,  100000000);
    QVector3D max(-100000000, -100000000, -100000000);
    if (_collider != nullptr) {
        QVector3D cMin = _collider->_localAABB._min;
        QVector3D cMax = _collider->_localAABB._max;

        //Here we make a local Cube from a local AABB
        std::vector<QVector3D> cube;
        cube.push_back(QVector3D(cMin.x(), cMin.y(), cMin.z()));
        cube.push_back(QVector3D(cMax.x(), cMin.y(), cMin.z()));
        cube.push_back(QVector3D(cMin.x(), cMax.y(), cMin.z()));
        cube.push_back(QVector3D(cMax.x(), cMax.y(), cMin.z()));
        cube.push_back(QVector3D(cMin.x(), cMin.y(), cMax.z()));
        cube.push_back(QVector3D(cMax.x(), cMin.y(), cMax.z()));
        cube.push_back(QVector3D(cMin.x(), cMax.y(), cMax.z()));
        cube.push_back(QVector3D(cMax.x(), cMax.y(), cMax.z()));

        //Then we transform that local cube into a world Cube
        for (QVector3D& point : cube) {
            point = _transform->TransformMatrix() * point;
        }

        //we process a new AABB on that cube. This is an aproximation but way faster than processing an AABB on a mesh each frame. That new AABB will be in global space
        //Here we basiacally make the AABB of an oriented box
        for (QVector3D& point : cube) {
            min.setX(std::min(min.x(), point.x()));
            min.setY(std::min(min.y(), point.y()));
            min.setZ(std::min(min.z(), point.z()));
            max.setX(std::max(max.x(), point.x()));
            max.setY(std::max(max.y(), point.y()));
            max.setZ(std::max(max.z(), point.z()));
        }

        //Save the _personalGlobalAABB for collision check
        if (_personalGlobalAABB != nullptr) delete _personalGlobalAABB;
        _personalGlobalAABB = new AABB(min, max);
    }

    //Finally, we do that recursively for each child. The _globalAABB of a game object contains itself and all children
    for(GameObject* child : *_children) {
        child->RefreshAABB();
        AABB* current = child->_globalAABB;

        if (current != nullptr) {
            min.setX(std::min(min.x(), current->_min.x()));
            min.setY(std::min(min.y(), current->_min.y()));
            min.setZ(std::min(min.z(), current->_min.z()));
            max.setX(std::max(max.x(), current->_max.x()));
            max.setY(std::max(max.y(), current->_max.y()));
            max.setZ(std::max(max.z(), current->_max.z()));
        }
    }

    if (min.x() < max.x()) {
        delete _globalAABB;
        _globalAABB = new AABB(min, max);
    }
}

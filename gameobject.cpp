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

RenderData* GameObject::GetRenderData() {
    return _renderData;
}

void GameObject::AddComponent(Component *component) {
    _components->push_back(component);
}

AABB* GameObject::RefreshAABB() {
    QVector3D min( 100000000,  100000000,  100000000);
    QVector3D max(-100000000, -100000000, -100000000);
    if (_collider != nullptr) {
        QVector3D cMin = _collider->_AABB._min;
        QVector3D cMax = _collider->_AABB._max;

        //Here we transform a local AABB into a local Cube
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
        //TODO



        //We then process a new AABB on that cube. This is an aproximation but way faster than processing an AABB on a mesh
        //TODO


        // min =
        // max =
    }

    for(GameObject* child : *_children) {
        AABB* current = child->RefreshAABB();

        if (current != nullptr) {
            min.setX(std::min(min.x(), current->_min.x()));
            min.setY(std::min(min.y(), current->_min.y()));
            min.setZ(std::min(min.z(), current->_min.z()));
            max.setX(std::max(max.x(), current->_max.x()));
            max.setY(std::max(max.y(), current->_max.y()));
            max.setZ(std::max(max.z(), current->_max.z()));
        }

        delete current;
    }

    if (min.x() < max.x()) {
        delete _AABB;
        _AABB = new AABB(min, max);
        return new AABB(min, max);
    }

    return nullptr;
}

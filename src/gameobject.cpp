#include "gameobject.h"


GameObject* GameObject::Root = nullptr;


GameObject::GameObject(std::string name, GameObject* parent, bool enabled) : _parent(parent) {
    this->_name = name;
    this->_enabled = enabled;

    if (parent == nullptr && Root == nullptr) {
        Root = this;
    }

    if (_transform != nullptr)
        delete _transform;
    _transform = new Transform(this);

    if (parent != nullptr) {
        parent->_children.insert(std::pair<std::string, GameObject*>(this->_name, this));
    }
}

GameObject::~GameObject() {
    //    std::cout << "Deleting '" << _name << "' GameObject..." << std::endl;
    if (_transform != nullptr)
        delete _transform;
    if (_collider != nullptr)
        delete _collider;
    if (_globalAABB != nullptr)
        delete _globalAABB;
    if (_personalGlobalAABB != nullptr)
        delete _personalGlobalAABB;

    for (auto* component : _components) {
        if (component != nullptr)
            delete component;
    }

    for (auto it = _children.begin(); it != _children.end(); it++) {
        GameObject* child = it->second;
        if (child != nullptr)
            delete child;
    }

    _parent->DeleteChildByName(_name);
}

//GETTERS SETTERS
bool GameObject::Enabled() {
    return _enabled;
}

Transform* GameObject::GetTransform() {
    return _transform;
}

void GameObject::SetTransform(Transform* t) {
    if (_transform != nullptr)
        delete _transform;
    _transform = new Transform(t, this);
}

GameObject* GameObject::GetParent() {
    return _parent;
}

std::map<std::string, GameObject*> GameObject::GetChildren() {
    return _children;
}

GameObject* GameObject::GetChildByName(std::string name) {
    auto it = _children.find(name);

    if (it == _children.end()) {
        return nullptr;
    }
    else {
        return it->second;
    }
}

void GameObject::DeleteChildByName(std::string childName) {
    _children.erase(childName);
}

void GameObject::SetCollider(Collider *collider) {
    if (_collider != nullptr)
        delete _collider;
    _collider = collider;
    _collider->_parent = this;
    _collider->Init();
}

//METHODS
void GameObject::Enable() {
    _enabled = true;
}
void GameObject::Disable() {
    _enabled = false;
}

void GameObject::Start() {
    if (!_enabled || _started) return;

    for (auto* component : _components) {
        component->Start();
    }
    _started = true;
}

void GameObject::Update(float delta) {
    if (!_started) Start();

    for (auto* component : _components) {
        component->Update(delta);
    }
}

void GameObject::FixedUpdate(float delta) {
    if (!_started) Start();

    for (auto* component : _components) {
        component->FixedUpdate(delta);
    }
}

void GameObject::Collisions(GameObject* current) {
    if (current->Enabled() && current->_globalAABB != nullptr) { //We can collide with this branch
        //qDebug("%s", ("Testing " + NAME + " - " + current->NAME).c_str());
        if (_personalGlobalAABB->intersect(*current->_globalAABB)) { //We are "Colliding" with that branch, check current and its children !
            //qDebug("%s", (NAME + " is colliding with the " + current->NAME + " branch").c_str());
            if (current != this && current->_personalGlobalAABB != nullptr && _personalGlobalAABB->intersect(*current->_personalGlobalAABB)) { //Current collision
                //qDebug("%s", (NAME + " is colliding with " + current->NAME + " (aabb)").c_str());
                if (_collider->Collision(current->GetCollider())) {
                    //qDebug("%s", (NAME + " is colliding with " + current->NAME).c_str());
                    //NOTE: now if A collide with B, only A is notified as B will be computed later
                    //Possible optimisation : notify A and B, and mark that collision as solved. When we test B to A, skip the test
                    for (auto* c : _components) {
                        c->Collision(current->GetCollider());
                    }
                }
            }
            for (auto it = _children.begin(); it != _children.end(); it++) { //Possible children collision
                GameObject* child = it->second;
                Collisions(child);
            }
        }
    }
}

RayCastHit GameObject::AABBRayCollision(QVector3D origin, QVector3D direction) {
    if (_globalAABB == nullptr) return RayCastHit(); //No GlobalAABB, No collision

    RayCastHit hit;
    hit._distance = _globalAABB->RayIntersect(origin, direction);
    if (hit._distance <= 0) return RayCastHit(); //Ray missed GlobalAABB, No collision

    hit._distance = std::numeric_limits<float>::max();

    if (_personalGlobalAABB) {
        hit._distance = _personalGlobalAABB->RayIntersect(origin, direction);
        if (hit._distance > 0) { //Ray hit the personal AABB, possible collision
            //TODO, proper Ray / Collider test
            if (true) {
                hit._gameobject = this;
                hit._position = origin + direction * hit._distance;
            }
        }
    }

    for (auto it = _children.begin(); it != _children.end(); it++) {
        GameObject* child = it->second;
        RayCastHit tmp;
        tmp = child->AABBRayCollision(origin, direction);
        if (tmp._distance > 0) {
            if (hit._distance <= 0 || tmp._distance < hit._distance) {
                hit = tmp;
            }
        }
    }

    return hit;
}

void GameObject::AddComponent(Component *component) {
    _components.push_back(component);
}

void GameObject::RefreshAABB() {
    int minInt = std::numeric_limits<int>::min();
    int maxInt = std::numeric_limits<int>::max();
    QVector3D min(maxInt, maxInt, maxInt);
    QVector3D max(minInt, minInt, minInt);

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
            point = _transform->GlobalTransformMatrix() * point;
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
        if (_personalGlobalAABB != nullptr)
            delete _personalGlobalAABB;
        _personalGlobalAABB = new AABB(min, max);
        //qDebug("%s", ("For " + NAME + ", personalAABB is " + std::to_string(_personalGlobalAABB->_min.x()) + ", " + std::to_string(_personalGlobalAABB->_min.y()) + ", " + std::to_string(_personalGlobalAABB->_min.z()) + " : " + std::to_string(_personalGlobalAABB->_max.x()) + ", " + std::to_string(_personalGlobalAABB->_max.y()) + ", " + std::to_string(_personalGlobalAABB->_max.z())).c_str());
    }

    //Finally, we do that recursively for each child. The _globalAABB of a game object contains itself and all children
    for (auto it = _children.begin(); it != _children.end(); it++) {
        GameObject* child = it->second;
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
        if (_globalAABB != nullptr)
            delete _globalAABB;
        _globalAABB = new AABB(min, max);
        //qDebug("%s", ("For " + NAME + ", globalAABB is " + std::to_string(_globalAABB->_min.x()) + ", " + std::to_string(_globalAABB->_min.y()) + ", " + std::to_string(_globalAABB->_min.z()) + " : " + std::to_string(_globalAABB->_max.x()) + ", " + std::to_string(_globalAABB->_max.y()) + ", " + std::to_string(_globalAABB->_max.z())).c_str());
    }
}


void GameObject::Draw() {
    if (!_started) Start();

    for (auto* c : _components) {
        c->Draw();
    }
}

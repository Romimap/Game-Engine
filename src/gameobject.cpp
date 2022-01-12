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

    if (_parent != nullptr)
        _parent->DeleteChildByName(_name);
}

//GETTERS SETTERS
///Returns true if the GameObject is enabled
bool GameObject::Enabled() {
    return _enabled;
}

///Returns the transform of this GameObject
Transform* GameObject::GetTransform() {
    return _transform;
}

///Sets the transform for this GameObject (Previous transform is deleted !)
void GameObject::SetTransform(Transform* t) {
    if (_transform != nullptr)
        delete _transform;
    _transform = new Transform(t, this);
}

///Returns the parent of this GameObject
GameObject* GameObject::GetParent() {
    return _parent;
}

///Returns the children of this GameObject
std::map<std::string, GameObject*> GameObject::GetChildren() {
    return _children;
}

///Returns the first child named <name>, or nullptr if it does not exists
GameObject* GameObject::GetChildByName(std::string name) {
    auto it = _children.find(name);

    if (it == _children.end()) {
        return nullptr;
    }
    else {
        return it->second;
    }
}

///Deletes the first child named <childName>
void GameObject::DeleteChildByName(std::string childName) {
    _children.erase(childName);
}

///Sets the collider for that gameObject
void GameObject::SetCollider(Collider *collider) {
    if (_collider != nullptr)
        delete _collider;
    _collider = collider;
    _collider->_parent = this;
    _collider->Init();
}

//METHODS
///Enables this GameObject
void GameObject::Enable() {
    _enabled = true;
}
///Disables this GameObject
void GameObject::Disable() {
    _enabled = false;
}

///Starts this GameObject, Calls Start on all Components. Called on the first update
void GameObject::Start() {
    if (!_enabled || _started) return;

    for (auto* component : _components) {
        component->Start();
    }
    _started = true;
}

///Updates this GameObject. Calls Update on all Components
void GameObject::Update(float delta) {
    if (!_started) Start();

    for (auto* component : _components) {
        component->Update(delta);
    }
}

///FixedUpdate for this GameObject. Calls FixedUpdate on all Components
void GameObject::FixedUpdate(float delta) {
    if (!_started) Start();

    for (auto* component : _components) {
        component->FixedUpdate(delta);
    }
}

///Recursively checks if this GameObjects collides with every other GameObject on the scene.
///Collision checks are done first via BVH, then refined using a Collider to Collider test.
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

///Recursively Checks if a ray intersects with the bounding boxes of GameObjects in the scene.
///Fills <gameObjectDistance> with the possible collisions.
void GameObject::AABBRayCollision(QVector3D origin, QVector3D direction, std::vector<std::pair<GameObject*, float>> *gameObjectDistance) {
    if (_globalAABB == nullptr) return; //No GlobalAABB, No collision

    float distance;
    distance = _globalAABB->RayIntersect(origin, direction);
    if (distance < std::numeric_limits<float>::max()) { //Hit global AABB, process self & children
        distance = std::numeric_limits<float>::max();

        if (_personalGlobalAABB != nullptr) {
            distance = _personalGlobalAABB->RayIntersect(origin, direction);
            if (distance < std::numeric_limits<float>::max()) { //Ray hit the personal AABB, possible collision
                gameObjectDistance->push_back(std::pair<GameObject*, float>(this, distance));
            }
        }

        for (std::pair<std::string, GameObject*> child : GetChildren()) {
            child.second->AABBRayCollision(origin, direction, gameObjectDistance);
        }
    }
}

///Adds a Component to our GameObject
void GameObject::AddComponent(Component *component) {
    _components.push_back(component);
}

///Refreshes AABB to make sure that the BVH is healthy.
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

///Draws this GameObject, Calls Draw on all Components.
void GameObject::Draw() {
    if (!_started) Start();

    for (auto* c : _components) {
        c->Draw();
    }
}

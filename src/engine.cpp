#include "engine.h"


Engine* Engine::Singleton = nullptr;

Engine::Engine(int width, int height) {
    this->_width = width;
    this->_height = height;

    if (Singleton != nullptr)
        delete Singleton;
    Singleton = this;
}

void Engine::initializeGL() {
    qDebug("initializeGL <-");
    initializeOpenGLFunctions();

    resize(_width, _height);

    glClearColor(0.212f, 0.224f, 0.247f, 1); // Discord gray

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
    qDebug("initializeGL ->");

    // Initialize the input manager
    InputManager::SampleMousePosition();
    InputManager::NextFrame();

    // Initialize the gameobjects already present in the tree
    Start(GameObject::Root);

    //Start timer
    _beginTime = std::chrono::system_clock::now();
    _lastTime = _beginTime;
    _fixedDeltaTime = 0;
}

void Engine::resizeGL(int w, int h) {
    Camera::ActiveCamera->ResizeGL(w, h);
}

void Engine::paintGL() {
    //Could be ignored (colors anyways)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Time
    std::chrono::time_point<std::chrono::system_clock> _currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double> deltaTimeDuration = _currentTime - _lastTime;
    double deltaTime = deltaTimeDuration.count();
    _fixedDeltaTime += deltaTime;
    _lastTime = _currentTime;

    //Inputs
    InputManager::SampleMousePosition();

    // Set InputManager Data
    InputManager::NextFrame();

    // Compute Collisions
    GameObject::Root->RefreshAABB();
    Collisions(GameObject::Root);

    // Update GameObjects
    std::cout << "#####   CALL TO UPDATE   #####" << std::endl;
    Update(GameObject::Root, deltaTime);
    while (_fixedDeltaTime > _fixedUpdateLen) {
        _fixedDeltaTime -= _fixedUpdateLen;
        FixedUpdate(GameObject::Root, _fixedUpdateLen);
    }

    // Draw GameObjects
    Draw(GameObject::Root);

    //    qDebug((std::to_string(deltaTime) + " (engine.cpp)").c_str());

    update();
}

void Engine::Draw(GameObject* current) {
    current->Draw();
    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Draw(child);
    }
}

void Engine::Start(GameObject* current) {
    if (!current->Enabled()) return;

    current->Start();

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Start(child);
    }
}

void Engine::Update(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->Update(deltaTime);

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Update(child, deltaTime);
    }
}

void Engine::FixedUpdate(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->FixedUpdate(deltaTime);

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        FixedUpdate(child, deltaTime);
    }
}

void Engine::Collisions(GameObject* current) {
    if (current->Enabled()) {
        if (current->GetPersonalGlobalAABB() != nullptr) current->Collisions(GameObject::Root);

        std::map<std::string, GameObject*> children = current->GetChildren();
        for (auto it = children.begin(); it != children.end(); it++) {
            GameObject* child = it->second;
            Collisions(child);
        }
    }
}

void Engine::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()){
        InputManager::Press(event->key());
    }
}

void Engine::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()){
        InputManager::Resease(event->key());
    }
}

RayCastHit Engine::RayCast(QVector3D origin, QVector3D direction) {
    RayCastHit hit = GameObject::Root->AABBRayCollision(origin, direction);
    if (hit._gameobject != nullptr) {
        GameObject* gameobject = hit._gameobject;
        hit = gameobject->GetCollider()->RayCast(origin, direction);
        if (hit._distance < std::numeric_limits<float>::max()) {
            hit._gameobject = gameobject;
        }

        return hit;
    }
    return hit;
}

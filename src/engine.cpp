#include "engine.h"
#include "inputmanager.h"

Engine* Engine::Singleton = nullptr;

Engine::Engine() {
    if (Singleton != nullptr)
        delete Singleton;
    Singleton = this;
}

void Engine::initializeGL() {
    qDebug("initializeGL <-");
    initializeOpenGLFunctions();

    //resize(1280, 720);
    resize(640, 360);

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
    for (GameObject* child : current->GetChildren()) {
        Draw(child);
    }
}

void Engine::Start(GameObject* current) {
    if (!current->Enabled()) return;

    current->Start();
    for (GameObject* child : current->GetChildren()) {
        Start(child);
    }
}

void Engine::Update(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->Update(deltaTime);
    for (GameObject* child : current->GetChildren()) {
        Update(child, deltaTime);
    }
}

void Engine::FixedUpdate(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->FixedUpdate(deltaTime);
    for (GameObject* child : current->GetChildren()) {
        FixedUpdate(child, deltaTime);
    }
}

void Engine::Collisions(GameObject* current) {
    if (current->Enabled()) {
        if (current->GetPersonalGlobalAABB() != nullptr) current->Collisions(GameObject::Root);
        for (GameObject* child : current->GetChildren()) {
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
    std::cout << "RAY ! " << hit._distance << std::endl;
    return hit;
}


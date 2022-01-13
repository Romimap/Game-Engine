#include "engine.h"
#include <map>

Engine* Engine::Singleton = nullptr;

Engine::Engine(int width, int height) {
    this->_width = width;
    this->_height = height;
    this->_lastWindowModeChange = std::chrono::high_resolution_clock::now();

    if (Singleton != nullptr)
        delete Singleton;
    Singleton = this;
}

///Inits the engine, here OpenGL, the InputManager and the timers are initialized
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

///Called on window resize. Tells the active camera about the changes.
void Engine::resizeGL(int w, int h) {
    Camera::ActiveCamera->ResizeGL(w, h);
}

///Called on refresh. This function is used as our gameloop.
void Engine::paintGL() {
    if (InputManager::Key('W')) {
        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - _lastWindowModeChange;
        if (elapsed.count() > 0.5) {
            if (isMaximized())
                showNormal();
            else
                showMaximized();
            _lastWindowModeChange = std::chrono::high_resolution_clock::now();
        }
    }

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

///Emits a Draw event on all GameObjects.
void Engine::Draw(GameObject* current) {
    current->Draw();
    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Draw(child);
    }
}


///Emits a Start event on all GameObjects
void Engine::Start(GameObject* current) {
    if (!current->Enabled()) return;

    current->Start();

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Start(child);
    }
}

///Emits an Update event on all GameObjects
void Engine::Update(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->Update(deltaTime);

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        Update(child, deltaTime);
    }
}

///Emits a FixedUpdate event on all GameObjects
void Engine::FixedUpdate(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->FixedUpdate(deltaTime);

    std::map<std::string, GameObject*> children = current->GetChildren();
    for (auto it = children.begin(); it != children.end(); it++) {
        GameObject* child = it->second;
        FixedUpdate(child, deltaTime);
    }
}

///Emits a Collision check event on all GameObjects
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

///Called when a key is pressed, forwards that event to the InputManager
void Engine::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()){
        InputManager::Press(event->key());
    }
}

///Called when a key is released, forwards that event to the InputManager
void Engine::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()){
        InputManager::Resease(event->key());
    }
}

///Comparator used in the Engine::RayCast() function
bool cmp(std::pair<GameObject*, float> &a,
         std::pair<GameObject*, float> &b) {
    return a.second < b.second;
}

///Casts a ray to our scene. Returns either data about a collision, or an "Empty" RayCastHit
RayCastHit Engine::RayCast(QVector3D origin, QVector3D direction) {
    std::vector<std::pair<GameObject*, float>> gameObjectDistance;

    GameObject::Root->AABBRayCollision(origin, direction, &gameObjectDistance);

    std::sort(gameObjectDistance.begin(), gameObjectDistance.end(), cmp);

    for (int i = 0; i < gameObjectDistance.size(); i++) {
        GameObject *current = gameObjectDistance[i].first;
        if (current != nullptr && current->GetCollider() != nullptr) {
            RayCastHit hit = current->GetCollider()->RayCast(origin, direction);
            if (hit._distance < std::numeric_limits<float>::max()) {
                hit._gameobject = gameObjectDistance[i].first;
                hit._position = origin + direction * hit._distance;
                return hit;
            }
        }
    }

    return RayCastHit();
}

#include "engine.h"
#include "inputmanager.h"

Engine* Engine::Singleton = nullptr;

Engine::Engine() {
    Singleton = this;
}

void Engine::initializeGL() {
    qDebug("initializeGL <-");
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
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
    qDebug("resizeGL <-");
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 100.0, fov = 85.0;

    // Reset projection
    _projection.setToIdentity();

    // Set perspective projection
    _projection.perspective(fov, aspect, zNear, zFar);
    qDebug("resizeGL ->");
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

    tick++;
    qDebug("%f", _fixedDeltaTime);
    qDebug("%f", deltaTime);


    update();
}

void Engine::Draw(GameObject* current) {
    // Clear color and depth buffer
    RenderData* renderData = current->GetRenderData();
    if (renderData != nullptr) {
        renderData->_material->color->bind(0);
        renderData->_material->normal->bind(1);
        renderData->_material->program.bind();

        //NOTE: for some reason the camera matrix needs to be inverted
        QMatrix4x4 cameraMatrix = Camera::ActiveCamera->GetTransform()->GlobalTransformMatrix().inverted();
        QMatrix4x4 modelMatrix = current->GetTransform()->GlobalTransformMatrix();

        // Set modelview-projection matrix
        renderData->_material->program.setUniformValue("mvp_matrix", _projection * cameraMatrix * modelMatrix);
        renderData->_material->program.setUniformValue("projection_matrix", _projection);
        renderData->_material->program.setUniformValue("view_matrix", cameraMatrix);
        renderData->_material->program.setUniformValue("model_matrix", modelMatrix);

        // Use texture unit 0
        renderData->_material->program.setUniformValue("color", 0);
        // Use texture unit 1
        renderData->_material->program.setUniformValue("normal", 1);

        // Draw Mesh
        float distance = (Camera::ActiveCamera->GetTransform()->GetGlobalPosition() - current->GetTransform()->GetGlobalPosition()).length();
        qDebug("%s", ("Distance " + current->NAME + " = " + std::to_string(distance)).c_str());
        renderData->_mesh->draw(&current->GetRenderData()->_material->program, distance);
    }

    for (GameObject* child : *current->GetChildren()) {
        Draw(child);
    }
}

void Engine::Start(GameObject* current) {
    if (!current->Enabled()) return;

    current->Start();
    for (GameObject* child : *current->GetChildren()) {
        Start(child);
    }
}

void Engine::Update(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->Update(deltaTime);
    for (GameObject* child : *current->GetChildren()) {
        Update(child, deltaTime);
    }
}

void Engine::FixedUpdate(GameObject* current, double deltaTime) {
    if (!current->Enabled()) return;

    current->FixedUpdate(deltaTime);
    for (GameObject* child : *current->GetChildren()) {
        FixedUpdate(child, deltaTime);
    }
}

void Engine::Collisions(GameObject* current) {
    if (current->Enabled()) {
        if (current->GetPersonalGlobalAABB() != nullptr) current->Collisions(GameObject::Root);
        for (GameObject* child : *current->GetChildren()) {
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


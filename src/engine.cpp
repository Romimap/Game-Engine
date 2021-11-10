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
    //glEnable(GL_CULL_FACE);
    qDebug("initializeGL ->");


    // Initialize the gameobjects already present in the tree
    Start(GameObject::Root);

    //Start timer
    _beginTime = std::chrono::system_clock::now();
    _lastTime = _beginTime;
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
    std::chrono::duration<double> deltaTimeDuration = _lastTime - _currentTime;
    double deltaTime = deltaTimeDuration.count();
    _lastTime = _currentTime;

    // Set InputManager Data
    InputManager::NextFrame();

    // Compute Collisions
    GameObject::Root->RefreshAABB();
    Collisions(GameObject::Root);

    // Update GameObjects
    Update(GameObject::Root, deltaTime);

    // Draw GameObjects
    Draw(GameObject::Root);

    update();
}

void Engine::Draw(GameObject* current) {
    // Clear color and depth buffer
    RenderData* renderData = current->GetRenderData();
    if (renderData != nullptr) {
        renderData->_material->color->bind();
        renderData->_material->program.bind();

        QMatrix4x4 cameraMatrix = Camera::ActiveCamera->GetTransform()->TransformMatrix();
        QMatrix4x4 modelMatrix = current->GetTransform()->TransformMatrix();

        // Set modelview-projection matrix
        renderData->_material->program.setUniformValue("mvp_matrix", _projection * cameraMatrix * modelMatrix);
        renderData->_material->program.setUniformValue("projection_matrix", _projection);
        renderData->_material->program.setUniformValue("view_matrix", cameraMatrix);
        renderData->_material->program.setUniformValue("model_matrix", modelMatrix);

        // Use texture unit 0
        renderData->_material->program.setUniformValue("color", 0);

        // Draw cube geometry
        renderData->_mesh->draw(&current->GetRenderData()->_material->program);
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


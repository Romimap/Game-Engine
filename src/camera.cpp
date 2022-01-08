#include "camera.h"


Camera* Camera::ActiveCamera = nullptr;


Camera::Camera(std::string name, int renderDistance, GameObject* parent) : GameObject(name, parent) {
    this->_renderDistance = renderDistance;

    if (ActiveCamera == nullptr)
        ActiveCamera = this;
}

void Camera::ResizeGL(int w, int h) {
    qDebug("resizeGL <-");
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 1000.0, fov = 48.0; //TODO: this but as a member

    // Reset projection
    _projectionMatrix.setToIdentity();

    // Set perspective projection
    _projectionMatrix.perspective(fov, aspect, zNear, zFar);

    _w = w;
    _h = h;

    qDebug("resizeGL ->");
}

void Camera::SetActive() {
    ActiveCamera = this;
}

QVector2D Camera::ScreenSize() {
    return QVector2D(_w, _h);
}

//TODO
QVector2D Camera::GetScreenRatios() {
    //Values for 85° FOV
    return QVector2D(0.91633111496 * (float(_w) / float(_h)), 0.91633111496);
}

int Camera::getRenderDistance() {
    return _renderDistance;
}

void Camera::setRenderDistance(int renderDistance) {
    this->_renderDistance = renderDistance;
}

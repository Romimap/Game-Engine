#include "camera.h"

Camera* Camera::ActiveCamera = nullptr;

Camera::Camera(GameObject* parent) : GameObject(parent) {
    if (ActiveCamera == nullptr)
        ActiveCamera = this;
}

void Camera::ResizeGL(int w, int h) {
    qDebug("resizeGL <-");
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 1000.0, fov = 55.0; //TODO: this but as a member

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
    return QVector2D(1.42814773744, float(_h) / float(_w) * 1.42814773744);
}

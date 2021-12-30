#include "camera.h"

Camera* Camera::ActiveCamera = nullptr;

Camera::Camera(GameObject* parent) : GameObject(parent) {
    if (ActiveCamera == nullptr)
        ActiveCamera = this;
}

void Camera::SetActive() {
    ActiveCamera = this;
}

void Camera::ResizeGL(int w, int h) {
    qDebug("resizeGL <-");
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = .1, zFar = 100.0, fov = 85.0; //TODO: this but as a member

    // Reset projection
    _projectionMatrix.setToIdentity();

    // Set perspective projection
    _projectionMatrix.perspective(fov, aspect, zNear, zFar);

    qDebug("resizeGL ->");
}

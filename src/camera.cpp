#include "src/header/camera.h"

Camera* Camera::ActiveCamera = nullptr;

Camera::Camera(GameObject* parent) : GameObject(parent) {
    if (ActiveCamera == nullptr)
        ActiveCamera = this;
}

void Camera::SetActive() {
    ActiveCamera = this;
}

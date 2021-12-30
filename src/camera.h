#ifndef CAMERA_H
#define CAMERA_H

#include "gameobject.h"

class Camera : public GameObject {
public:
    static Camera* ActiveCamera;
    QMatrix4x4 _projectionMatrix;
    Camera(GameObject* parent);
    void SetActive();
    void ResizeGL(int w, int h);
};

#endif // CAMERA_H

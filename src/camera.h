#ifndef CAMERA_H
#define CAMERA_H

#include "gameobject.h"

class Camera : public GameObject {
public:
    static Camera* ActiveCamera;
    Camera(GameObject* parent);
    void SetActive();
};

#endif // CAMERA_H

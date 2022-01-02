#ifndef CAMERA_H
#define CAMERA_H

#include "gameobject.h"
#include <QVector2D>

class Camera : public GameObject {
private:
    int _w;
    int _h;
public:
    static Camera* ActiveCamera;
    QMatrix4x4 _projectionMatrix;
    Camera(GameObject* parent);
    void SetActive();
    void ResizeGL(int w, int h);
    QVector2D ScreenSize();
    QVector2D GetScreenRatios();
};

#endif // CAMERA_H

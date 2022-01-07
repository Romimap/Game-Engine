#ifndef CAMERA_H
#define CAMERA_H


#include <QVector2D>

#include "src/gameobject.h"


class Camera : public GameObject {
    /*** ATTRIBUTES ***/
private:
    int _w;
    int _h;

public:
    static Camera* ActiveCamera;

    QMatrix4x4 _projectionMatrix;

    /*** METHODS ***/
public:
    Camera(std::string name, GameObject* parent);

    void ResizeGL(int w, int h);

    void SetActive();
    QVector2D ScreenSize();
    QVector2D GetScreenRatios();
};

#endif // CAMERA_H

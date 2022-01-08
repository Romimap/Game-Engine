#ifndef ENGINE_H
#define ENGINE_H


#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>

#include "src/camera.h"
#include "src/gameobject.h"
#include "src/glmesh.h"
#include "src/inputmanager.h"
#include "src/material.h"


struct VertexData {
    QVector3D position;
    QVector2D uv;
};


class GameObject;


class Engine : public QOpenGLWidget, protected QOpenGLFunctions_3_1 {
    /*** ATTRIBUTES ***/
private:
    int _width;
    int _height;

    QMatrix4x4 _projection;
    std::chrono::time_point<std::chrono::system_clock> _beginTime;
    std::chrono::time_point<std::chrono::system_clock> _lastTime;
    double _fixedDeltaTime;
    double _fixedUpdateLen = 1./60.;

public:
    static Engine* Singleton;
    int tick = 0;

    /*** METHODS ***/
public:
    Engine(int width = 640, int height = 360);

    RayCastHit RayCast(QVector3D origin, QVector3D direction);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void Draw(GameObject* current);
    void Start(GameObject* current);
    void Update(GameObject* current, double deltaTime);
    void FixedUpdate(GameObject* current, double deltaTime);
    void Collisions(GameObject* current);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    RayCastHit RayCastRecursive(QVector3D origin, QVector3D direction, GameObject *current);
};

#endif // ENGINE_H

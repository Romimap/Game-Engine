#ifndef ENGINE_H
#define ENGINE_H

#include "glmesh.h"
#include "material.h"
#include "gameobject.h"
#include "camera.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLBuffer>
#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <chrono>

struct VertexData {
    QVector3D position;
    QVector2D uv;
};

class GameObject;
class Engine : public QOpenGLWidget, protected QOpenGLFunctions_3_1 {
private:
    QMatrix4x4 _projection;
    std::chrono::time_point<std::chrono::system_clock> _beginTime;
    std::chrono::time_point<std::chrono::system_clock> _lastTime;
    double _fixedDeltaTime;
    double _fixedUpdateLen = 1./60.;

public:
    static Engine* Singleton;
    int tick = 0;

public:
    Engine();

public:
    RayCastHit RayCast(QVector3D origin, QVector3D direction);
private:
    RayCastHit RayCastRecursive(QVector3D origin, QVector3D direction, GameObject *current);

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
};

#endif // ENGINE_H

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

struct RenderData {
    GLMesh* _mesh;
    Material* _material;

    RenderData(Material* material, GLMesh* mesh) {
        _mesh = mesh;
        _material = material;
    }
};

class GameObject;
class Engine : public QOpenGLWidget, protected QOpenGLFunctions_3_1 {
private:
    QMatrix4x4 _projection;
    std::chrono::time_point<std::chrono::system_clock> _beginTime;
    std::chrono::time_point<std::chrono::system_clock> _lastTime;

public:
    static Engine* Singleton;

public:
    Engine();

public:
    void DrawScene();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void Draw(GameObject* root);
    void Start();
    void Update(double deltaTime);
    void Collisions();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // ENGINE_H

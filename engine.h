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
    void Start(GameObject* root);
    void Update(GameObject* root);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // ENGINE_H

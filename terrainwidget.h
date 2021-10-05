#ifndef TERRAINWIDGET_H
#define TERRAINWIDGET_H

#include "geometryengine.h"
#include "component.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QPoint>

class TerrainWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_1, public Component {
Q_OBJECT

    //MEMBERS
private:
    GeometryEngine *_geometries;
    QOpenGLTexture *_heightmap;
    QOpenGLTexture *_grassTex;
    QOpenGLTexture *_rockTex;
    QOpenGLTexture *_snowTex;
    QMatrix4x4 _projection;
    QOpenGLShaderProgram _program;
    QString _heightmappath;
    float _azimuth;
    float _elevation;
    QQuaternion _rotation;
    QBasicTimer _timer;
    bool _cameraFree = false;
    QPoint _previousMousePos;
    QMatrix4x4 _viewTransform;

    //CONSTRUCTORS
public:
    explicit TerrainWidget (QWidget *parent = 0, QString heightmap = ":/heightmap-1024x1024.png");
    ~TerrainWidget ();

    //GETTERS SETTERS

    //METHODS
protected:
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initializeGL() override;
    void timerEvent(QTimerEvent *e) override;
    void mouseMoveEvent(QMouseEvent* event) override;

public:
    void Enable() override;
    void Disable() override;

protected:
    void initTextures();
    void initShaders();
};

#endif // TERRAINWIDGET_H

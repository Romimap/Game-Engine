#include "terrainwidget.h"

class QOpenGLWidget;

TerrainWidget::TerrainWidget (QWidget *parent, QString heightmap) :
    QOpenGLWidget(parent),
    _geometries(0),
    _heightmap(0),
    _heightmappath(heightmap){
  _rotation = QQuaternion::fromEulerAngles(QVector3D(0, 0, 0));
  _viewTransform.translate(0, 0, -5);
}

TerrainWidget::~TerrainWidget () {
    makeCurrent();
    delete _heightmap;
    delete _grassTex;
    delete _rockTex;
    delete _snowTex;

    delete _geometries;
    doneCurrent();
}

void TerrainWidget::timerEvent(QTimerEvent *) {
    // Update rotation
    _azimuth += 1;
    _elevation = 45;
    _rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), _azimuth);
    _rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), _elevation) * _rotation;

    // Request an update
    update();
}

void TerrainWidget::mouseMoveEvent(QMouseEvent* event) {
    QVector2D delta;
    delta.setX(_previousMousePos.rx() - event->pos().rx());
    delta.setY(_previousMousePos.ry() - event->pos().ry());

    _previousMousePos = event->pos();

    //QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), delta.y());
    //rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), delta.x()) * rotation;
    //
    //_viewTransform.rotate(rotation);

}


void TerrainWidget::resizeGL(int w, int h) {
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    _projection.setToIdentity();

    // Set perspective projection
    _projection.perspective(fov, aspect, zNear, zFar);
}


void TerrainWidget::paintGL() {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _heightmap->bind(0);
    _grassTex->bind(1);
    _rockTex->bind(2);
    _snowTex->bind(3);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 model;
    model.translate(0.0, 0.0, 0.0);
    model.rotate(_rotation);

    // Set modelview-projection matrix
    _program.setUniformValue("mvp_matrix", _projection * _viewTransform * model);
//! [6]

    _program.setUniformValue("heightmap", 0);
    _program.setUniformValue("grassTex", 1);
    _program.setUniformValue("rockTex", 2);
    _program.setUniformValue("snowTex", 3);

    // Draw cube geometry
    _geometries->draw(&_program);
}

void TerrainWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

//! [2]
    _geometries = new GeometryEngine(128, 128);
    _timer.start(12, this);
}
void TerrainWidget::initTextures() {
    // Load heightmap image
    _heightmap = new QOpenGLTexture(QImage(_heightmappath).mirrored());
    _grassTex =  new QOpenGLTexture(QImage(":/grass.png").mirrored());
    _rockTex =  new QOpenGLTexture(QImage(":/rock.png").mirrored());
    _snowTex =  new QOpenGLTexture(QImage(":/snowrocks.png").mirrored());

    // Set nearest filtering mode for texture minification
    _heightmap->setMinificationFilter(QOpenGLTexture::Nearest);
    _grassTex->setMinificationFilter(QOpenGLTexture::Nearest);
    _rockTex->setMinificationFilter(QOpenGLTexture::Nearest);
    _snowTex->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    _heightmap->setMagnificationFilter(QOpenGLTexture::Linear);
    _grassTex->setMagnificationFilter(QOpenGLTexture::Linear);
    _rockTex->setMagnificationFilter(QOpenGLTexture::Linear);
    _snowTex->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    _heightmap->setWrapMode(QOpenGLTexture::Repeat);
    _grassTex->setWrapMode(QOpenGLTexture::Repeat);
    _rockTex->setWrapMode(QOpenGLTexture::Repeat);
    _snowTex->setWrapMode(QOpenGLTexture::Repeat);

}

void TerrainWidget::initShaders() {
    // Compile vertex shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/terrainvshader.glsl"))
        close();

    // Compile fragment shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/terrainfshader.glsl"))
        close();

    // Link shader pipeline
    if (!_program.link())
        close();

    // Bind shader pipeline for use
    if (!_program.bind())
        close();
}

void TerrainWidget::Enable() {
    show();
}


void TerrainWidget::Disable() {
    hide();
}

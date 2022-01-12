/*
    GLMESH.H
    Stores a mesh that will be used in an OpenGL Draw Call.
*/

#ifndef GLMESH_H
#define GLMESH_H


#include <fstream>
#include <iostream>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>


class GLMesh : protected QOpenGLFunctions_3_1 {
    /*** ATTRIBUTES ***/
private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    /*** METHODS ***/
public:
    GLMesh(std::string lod0);

    void draw(QOpenGLShaderProgram *program);

private:
    void initMesh(std::string lod0, QOpenGLBuffer* arrayBuffer, QOpenGLBuffer* indexBuffer);
};

#endif // GLMESH_H

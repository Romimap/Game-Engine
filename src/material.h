#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material {
public:
    QOpenGLTexture* color;
    QOpenGLTexture* normal;
    QOpenGLShaderProgram program;
    Material(char* colorPath, char* normalPath, char* vshaderPath, char* fshaderPath);
};

#endif // MATERIAL_H

#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material {
public:
    QOpenGLTexture* colorLod8;
    QOpenGLTexture* colorLod64;
    QOpenGLTexture* colorLod1024;

    QOpenGLShaderProgram program;
    Material(char* colorPath, char* vshaderPath, char* fshaderPath);
};

#endif // MATERIAL_H

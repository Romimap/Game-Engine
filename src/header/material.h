#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material {
public:
    QOpenGLTexture* color;
    QOpenGLShaderProgram program;
    Material();
};

#endif // MATERIAL_H

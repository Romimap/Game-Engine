#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material {
public:
    QOpenGLTexture* _TexSlot0;
    QOpenGLTexture* _TexSlot1;
    QOpenGLTexture* _TexSlot2;
    QOpenGLTexture* _TexSlot3;
    QOpenGLTexture* _TexSlot4;
    QOpenGLTexture* _TexSlot5;
    QOpenGLTexture* _TexSlot6;
    QOpenGLTexture* _TexSlot7;

    QOpenGLShaderProgram program;
    Material(char* colorPath, char* vshaderPath, char* fshaderPath);
};

#endif // MATERIAL_H

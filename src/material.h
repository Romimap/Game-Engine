#ifndef MATERIAL_H
#define MATERIAL_H


#include <chrono>
#include <iostream>

#include <QColor>
#include <QImage>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


class Material {
    /*** ATTRIBUTES ***/
public:
    QOpenGLTexture* _TexSlot0 = nullptr;
    QOpenGLTexture* _TexSlot1 = nullptr;
    QOpenGLTexture* _TexSlot2 = nullptr;
    QOpenGLTexture* _TexSlot3 = nullptr;
    QOpenGLTexture* _TexSlot4 = nullptr;
    QOpenGLTexture* _TexSlot5 = nullptr;
    QOpenGLTexture* _TexSlot6 = nullptr;
    QOpenGLTexture* _TexSlot7 = nullptr;

    QOpenGLShaderProgram program;


    /*** METHODS ***/
public:
    Material(std::string vshaderPath, std::string fshaderPath);
    ~Material();

    void SetSlot2D(std::string path, int slot);
    QOpenGLTexture* SetSlot3D(int slot, int sizeX, int sizeY, int sizeZ, unsigned char* data);

protected:
    QOpenGLTexture* ApplyToSlot(QOpenGLTexture* tex, int slot);
};

#endif // MATERIAL_H

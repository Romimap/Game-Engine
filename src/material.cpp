#include "src/material.h"
#include <QOpenGLFunctions_3_1>
#include <QImage>
#include <QColor>

Material::Material(std::string vshaderPath, std::string fshaderPath) {

    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath.c_str());

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath.c_str());

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

Material::~Material() {
    if (_TexSlot0 != nullptr) delete _TexSlot0;
    if (_TexSlot1 != nullptr) delete _TexSlot1;
    if (_TexSlot2 != nullptr) delete _TexSlot2;
    if (_TexSlot3 != nullptr) delete _TexSlot3;
    if (_TexSlot4 != nullptr) delete _TexSlot4;
    if (_TexSlot5 != nullptr) delete _TexSlot5;
    if (_TexSlot6 != nullptr) delete _TexSlot6;
    if (_TexSlot7 != nullptr) delete _TexSlot7;
}

void Material::SetSlot2D(std::string path, int slot) {
    QOpenGLTexture* tex = new QOpenGLTexture(QImage(path.c_str()));
    tex->setMinMagFilters(QOpenGLTexture::Filter::Nearest, QOpenGLTexture::Filter::Nearest);
    ApplyToSlot(tex, slot);
}

QOpenGLTexture* Material::SetSlot3D(int slot, int sizeX, int sizeY, int sizeZ, unsigned char* data) {
    if (slot < 0 || slot > 7)
        return nullptr;

    QOpenGLTexture* tex = new QOpenGLTexture(QOpenGLTexture::Target3D);
    tex->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    tex->setWrapMode(QOpenGLTexture::Repeat);
    tex->create();
    tex->setSize(sizeX, sizeY, sizeZ);
    tex->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    tex->allocateStorage();
    tex->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data);

    return ApplyToSlot(tex, slot);
}

QOpenGLTexture* Material::ApplyToSlot(QOpenGLTexture* tex, int slot) {
    switch (slot) {
    case 0:
        if (_TexSlot0 != nullptr) delete _TexSlot0;
        _TexSlot0 = tex;
        return tex;
    case 1:
        if (_TexSlot1 != nullptr) delete _TexSlot1;
        _TexSlot1 = tex;
        return tex;
    case 2:
        if (_TexSlot2 != nullptr) delete _TexSlot2;
        _TexSlot2 = tex;
        return tex;
    case 3:
        if (_TexSlot3 != nullptr) delete _TexSlot3;
        _TexSlot3 = tex;
        return tex;
    case 4:
        if (_TexSlot4 != nullptr) delete _TexSlot4;
        _TexSlot4 = tex;
        return tex;
    case 5:
        if (_TexSlot5 != nullptr) delete _TexSlot5;
        _TexSlot5 = tex;
        return tex;
    case 6:
        if (_TexSlot6 != nullptr) delete _TexSlot6;
        _TexSlot6 = tex;
        return tex;
    case 7:
        if (_TexSlot7 != nullptr) delete _TexSlot7;
        _TexSlot7 = tex;
        return tex;
    default:
        std::cerr << "Undefined slot (" << slot << ")" << std::endl;
        delete tex;
        return nullptr;
    }
}

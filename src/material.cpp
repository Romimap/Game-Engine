#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {

    colorLod32 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod32->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod32->setWrapMode(QOpenGLTexture::Repeat);
    colorLod32->create();
    colorLod32->setSize(32, 32, 32);
    colorLod32->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod32->allocateStorage();
    char* data32 = (char*)malloc(32*32*32 * sizeof (char));
    for (int k = 0; k < 32*32*32; k++) {
        data32[k] = (rand() % 5 == 0) * 255;
    }
    colorLod32->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data32);


    colorLod1024 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod1024->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod1024->setWrapMode(QOpenGLTexture::Repeat);
    colorLod1024->create();
    colorLod1024->setSize(1024, 1024, 1024);
    colorLod1024->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod1024->allocateStorage();
    char* data1024 = (char*)malloc(1024*1024*1024 * sizeof (char));
    for (int k = 0; k < 1024*1024*1024; k++) {
        data1024[k] = (rand() % 30 == 0) * 255;
    }
    colorLod1024->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data1024);


    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath);

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath);

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {


    colorLod4 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod4->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod4->setWrapMode(QOpenGLTexture::Repeat);
    colorLod4->create();
    colorLod4->setSize(4, 4, 4);
    colorLod4->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod4->allocateStorage();
    char* data4 = (char*)malloc(4*4*4 * sizeof (char));
    for (int k = 0; k < 4*4*4; k++) {
        data4[k] = (rand() % 10 == 0) * 255;
    }
    colorLod4->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data4);


    colorLod16 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod16->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod16->setWrapMode(QOpenGLTexture::Repeat);
    colorLod16->create();
    colorLod16->setSize(16, 16, 16);
    colorLod16->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod16->allocateStorage();
    char* data16 = (char*)malloc(16*16*16 * sizeof (char));
    for (int k = 0; k < 16*16*16; k++) {
        data16[k] = (rand() % 10 == 0) * 255;
    }
    colorLod16->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data16);


    colorLod64 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod64->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod64->setWrapMode(QOpenGLTexture::Repeat);
    colorLod64->create();
    colorLod64->setSize(64, 64, 64);
    colorLod64->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod64->allocateStorage();
    char* data64 = (char*)malloc(64*64*64 * sizeof (char));
    for (int k = 0; k < 64*64*64; k++) {
        data64[k] = (rand() % 10 == 0) * 255;
    }
    colorLod64->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data64);



    colorLod256 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod256->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod256->setWrapMode(QOpenGLTexture::Repeat);
    colorLod256->create();
    colorLod256->setSize(256, 256, 256);
    colorLod256->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod256->allocateStorage();
    char* data256 = (char*)malloc(256*256*256 * sizeof (char));
    for (int k = 0; k < 256*256*256; k++) {
        data256[k] = (rand() % 10 == 0) * 255;
    }
    colorLod256->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data256);


    colorLod1024 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod1024->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod1024->setWrapMode(QOpenGLTexture::Repeat);
    colorLod1024->create();
    colorLod1024->setSize(1024, 1024, 1024);
    colorLod1024->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod1024->allocateStorage();
    char* data1024 = (char*)malloc(1024*1024*1024 * sizeof (char));
    for (int k = 0; k < 1024*1024*1024; k++) {
        data1024[k] = (rand() % 10 == 0) * 255;
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

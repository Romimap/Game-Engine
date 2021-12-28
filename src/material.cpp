#include "src/material.h"
#include <QOpenGLFunctions_3_1>
#include <QImage>
#include <QColor>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {
    QImage heightmap;
    heightmap.load(":/montagne.png");

    colorLod8 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod8->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod8->setWrapMode(QOpenGLTexture::Repeat);
    colorLod8->create();
    colorLod8->setSize(8, 8, 8);
    colorLod8->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod8->allocateStorage();
    char* data8 = (char*)malloc(8*8*8 * sizeof (char));
    for (int k = 0; k < 8*8*8; k++) {
        data8[k] = 0;
    }

    colorLod64 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod64->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod64->setWrapMode(QOpenGLTexture::Repeat);
    colorLod64->create();
    colorLod64->setSize(64, 64, 64);
    colorLod64->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod64->allocateStorage();
    char* data64 = (char*)malloc(64*64*64 * sizeof (char));
    for (int k = 0; k < 64*64*64; k++) {
        data64[k] = 0;
    }


    colorLod1024 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod1024->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod1024->setWrapMode(QOpenGLTexture::Repeat);
    colorLod1024->create();
    colorLod1024->setSize(1024, 1024, 1024);
    colorLod1024->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod1024->allocateStorage();
    char* data1024 = (char*)malloc(1024*1024*1024 * sizeof (char));
    for (int k = 0; k < 1024*1024*1024; k++) {
        int x =  k         % 1024;
        int y = (k / 1024) % 1024;
        int z = (k / 1024) / 1024;

        int x64 = x / 16;
        int y64 = y / 16;
        int z64 = z / 16;
        int k64 = x64 + 64*y64 + 64*64*z64;

        if (k64 >= 64*64*64) {
            printf("ERROR 64 k=%d x=%d y=%d z=%d\n", k64, x64, y64, z64);
            exit(0);
        }

        int x8 = x64 / 8;
        int y8 = y64 / 8;
        int z8 = z64 / 8;
        int k8 = x8 + 8*y8 + 8*8*z8;

        if (k8 >= 8*8*8) {
            printf("ERROR 8 k=%d x=%d y=%d z=%d\n", k8, x8, y8, z8);
            exit(0);
        }

        QColor c = heightmap.pixelColor(x, z);

        if (c.red() < (1023 - y) / 2) {
            data1024[k] = 0b11111111;
            data64[k64] = 0b11111111;
            data8[k8] = 0b11111111;
        } else {
            data1024[k] = 0;
        }
        //data1024[k] = 0b11111111;
        //data1024[k] = (rand() % 20 == 0) * 255;
    }

    colorLod8->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data8);
    colorLod64->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data64);
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

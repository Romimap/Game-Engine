#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {
    colorLod0 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod0->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod0->setWrapMode(QOpenGLTexture::Repeat);
    colorLod0->create();

    int s = 512;
    colorLod0->setSize(s, s, s);
    colorLod0->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod0->allocateStorage();

    char* data = (char*)malloc(s*s*s * sizeof (char));
    for (int k = 0; k < s*s*s; k++) {
        data[k] = (rand() % 10 == 0) * 255;
    }

    colorLod0->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, data);


    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath);

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath);

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

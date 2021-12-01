#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {
    colorLod0 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod0->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod0->setWrapMode(QOpenGLTexture::Repeat);
    colorLod0->create();

    colorLod0->setSize(8, 8, 8);
    colorLod0->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    colorLod0->allocateStorage();

    char* data = (char*)malloc(8*8*8 * sizeof (char));
    for (int k = 0; k < 8*8*8; k++) {
        data[k] = (rand() % 8 == 0) * 255;
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

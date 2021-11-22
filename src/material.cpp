#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {
    colorLod0 = new QOpenGLTexture(QOpenGLTexture::Target3D);
    colorLod0->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    colorLod0->create();

    colorLod0->setSize(2, 2, 2);
    colorLod0->setFormat(QOpenGLTexture::TextureFormat::RGBA8_UNorm);
    colorLod0->allocateStorage();

    uint8_t* data = (uint8_t*)calloc(8 * 4, sizeof (uint8_t));
    data[3]  = 255;
    data[7]  = 255;
    data[11] = 255;

    colorLod0->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, data);

    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath);

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath);

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

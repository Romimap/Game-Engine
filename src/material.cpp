#include "src/material.h"
#include <QOpenGLFunctions_3_1>

Material::Material(char* colorPath, char* normalPath, char* vshaderPath, char* fshaderPath) {
    // Load cube.png image
    if (colorPath != nullptr) color = new QOpenGLTexture(QImage(colorPath).mirrored());
    else color = new QOpenGLTexture(QImage(":/default.png"));

    if (normalPath != nullptr) normal = new QOpenGLTexture(QImage(normalPath).mirrored());
    else normal = new QOpenGLTexture(QImage(":/defaultnormal.png"));

    // Set nearest filtering mode for texture minification
    color->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    color->setMagnificationFilter(QOpenGLTexture::Nearest);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    color->setWrapMode(QOpenGLTexture::Repeat);

    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath);

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath);

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

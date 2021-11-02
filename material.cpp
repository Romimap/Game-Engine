#include "material.h"

Material::Material() {
    // Load cube.png image
    color = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    color->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    color->setMagnificationFilter(QOpenGLTexture::Nearest);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    color->setWrapMode(QOpenGLTexture::Repeat);

    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

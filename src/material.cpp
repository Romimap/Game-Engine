#include "src/material.h"
#include <QOpenGLFunctions_3_1>
#include <QImage>
#include <QColor>

Material::Material(char* colorPath, char* vshaderPath, char* fshaderPath) {

    // Compile vertex shader
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath);

    // Compile fragment shader
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath);

    // Link shader pipeline
    program.link();

    // Bind shader pipeline for use
    program.bind();
}

#ifndef MATERIAL3D_H
#define MATERIAL3D_H


#include <iostream>
#include <vector>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


class Material3D {
    /*** ATTRIBUTES ***/
public:
    QOpenGLShaderProgram _program;

    std::vector<QOpenGLTexture*> _textures;
    std::vector<unsigned int> _sizeX;
    std::vector<unsigned int> _sizeY;
    std::vector<unsigned int> _sizeZ;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Material3D(std::string vshaderPath, std::string fshaderPath);
    ~Material3D();

    /** TEXTURE MANAGEMENT **/
    unsigned int addTexture(std::vector<std::vector<std::vector<unsigned char>>> data);
    void bindTexture(int textureID, int bindID);
};

#endif // MATERIAL3D_H

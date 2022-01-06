#ifndef MATERIAL3D_H
#define MATERIAL3D_H


#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <iostream>
#include <vector>


using namespace std;


class Material3D {
    /*** ATTRIBUTES ***/
public:
    QOpenGLShaderProgram _program;

    vector<QOpenGLTexture*> _textures;
    vector<unsigned int> _sizeX;
    vector<unsigned int> _sizeY;
    vector<unsigned int> _sizeZ;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Material3D(std::string vshaderPath, std::string fshaderPath);
    ~Material3D();

    /** TEXTURE MANAGEMENT **/
    unsigned int addTexture(vector<vector<vector<unsigned char>>> data);
    void bindTexture(int textureIndex, int bindIndex);
};

#endif // MATERIAL3D_H

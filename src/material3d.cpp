#include "material3d.h"

Material3D::Material3D(std::string vshaderPath, std::string fshaderPath) {

    // Compile vertex shader
    _program.addShaderFromSourceFile(QOpenGLShader::Vertex, vshaderPath.c_str());

    // Compile fragment shader
    _program.addShaderFromSourceFile(QOpenGLShader::Fragment, fshaderPath.c_str());

    // Link shader pipeline
    _program.link();

    // Bind shader pipeline for use
    _program.bind();
}

unsigned int Material3D::addTexture(vector<vector<vector<unsigned char>>> data) {

    /** Determine sizes **/

    int sizeX = data.size();
    int sizeY = data[0].size();
    int sizeZ = data[0][0].size();

    /** Convert data from nested vectors to unsigned char* array **/

    unsigned char* convertedData = (unsigned char*)malloc(sizeX * sizeY * sizeZ * sizeof (unsigned char));

    unsigned int index = 0;
    for (int z = 0; z < sizeZ; z++) {
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                convertedData[index++] = data[x][y][z];
            }
        }
    }

    /** Create the texture and assign data to it **/

    QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target3D);
    texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->create();
    texture->setSize(sizeX, sizeY, sizeZ);
    texture->setFormat(QOpenGLTexture::TextureFormat::LuminanceFormat);
    texture->allocateStorage();
    texture->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, convertedData);

    /** Save the texture and its 3D dimensions to the Material3D **/

    _textures.push_back(texture);
    _sizeX.push_back(sizeX);
    _sizeY.push_back(sizeY);
    _sizeZ.push_back(sizeZ);

    return _sizeX.size() - 1; // Return the index at which we can find the texture and its dimensions
}

void Material3D::bindTexture(int textureID, int bindID) {
    _textures[textureID]->bind(bindID);
}

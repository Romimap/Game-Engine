#include "chunk.h"


Chunk::Chunk(int x, int y, int z, int xSize, int ySize, int zSize, int nbOfLayers, int precisionFactor, const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence) {
    this->_hasChanged = true;
    this->_xSize = xSize;
    this->_ySize = ySize;
    this->_zSize = zSize;
    this->_precisionFactor = precisionFactor;
    this->_x = x;
    this->_y = y;
    this->_z = z;

    float xFrequency = frequency / (float) _xSize;
    float zFrequency = frequency / (float) _zSize;

    auto ***layer_x_y_z = new unsigned char**[_xSize];
    for (int x = 0; x < _xSize; x++) {
        auto **layer_1_y_z = new unsigned char*[_zSize];

        for (int z = 0; z < _zSize; z++) {
            const double noise = perlin.octave2D_01((x * xFrequency), (z * zFrequency), octaves, persistence);
            int groundLevel = (int)(noise * (_ySize - 1));

//            cout << "noise=" << noise << ", groundLevel=" << groundLevel << endl;

            auto *layer_1_y_1 = new unsigned char[_ySize];

            // Place stone
            for (int y = 0; y < groundLevel - 2; y++) {
                layer_1_y_1[y] = STONE;
            }

            // Place 2 dirt and 1 grass
            if (groundLevel > 1) layer_1_y_1[groundLevel - 2] = DIRT;
            if (groundLevel > 0) layer_1_y_1[groundLevel - 1] = DIRT;
            layer_1_y_1[groundLevel] = GRASS;

            // Place air above
            for (int y = groundLevel + 1; y < _ySize; y++) {
                layer_1_y_1[y] = AIR;
            }

            layer_1_y_z[z] = layer_1_y_1;
        }

        layer_x_y_z[x] = layer_1_y_z;
    }


//    for (int y = 0; y < _ySize; y++) {
//        cout << "### Y = " << y << " ###" << endl;
//        for (int x = 0; x < _xSize; x++) {
//            for (int z = 0; z < _zSize; z++) {
//                cout << (int)layer_x_y_z[x][z][y] << " ";
//            }
//            cout << endl;
//        }
//        cout << endl;
//    }

    _layers.push_back(layer_x_y_z);
}

Chunk::~Chunk() {
    int currentFactor = 1;
    for (auto layer : _layers) {
        for (int x = 0; x < _xSize / currentFactor; x++) {
            for (int z = 0; z < _zSize / currentFactor; z++) {
                delete[] layer[x][z];
            }
            delete[] layer[x];
        }
        delete[] layer;

        currentFactor *= _precisionFactor;
    }
}

unsigned char Chunk::getVoxel(int x, int y, int z) {
    return _layers[0][x][z][y];
}

/** Returns:
 *  -1 = error
 *   0 = voxel not modified
 *   1 = voxel modified
 **/
int Chunk::setVoxel(int x, int y, int z, unsigned char voxelMaterial) {
    if (x < 0 || x >= _xSize) return -1;
    if (y < 0 || y >= _ySize) return -1;
    if (z < 0 || z >= _zSize) return -1;

    _layers[0][x][z][y] = voxelMaterial;
    return 1;
}

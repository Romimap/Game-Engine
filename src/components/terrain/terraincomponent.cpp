#include "terraincomponent.h"


TerrainComponent::TerrainComponent(int chunkX, int chunkY, int chunkZ, int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor, GameObject* parent)
    : Component(parent) {

    this->_name = "TerrainComponent";
    this->_hasChanged = true;
    this->_xSize = xSize;
    this->_ySize = ySize;
    this->_zSize = zSize;
    this->_layerSizeReductionFactor = layerSizeReductionFactor;
    this->_chunkX = chunkX;
    this->_chunkY = chunkY;
    this->_chunkZ = chunkZ;

    /** Setup chunk GameObject's data **/

    QVector3D aabbMin(_chunkX * _xSize, _chunkY * _ySize, _chunkZ * _zSize);
    QVector3D aabbMax((_chunkX + 1) * _xSize, (_chunkY + 1) * _ySize, (_chunkZ + 1) * _zSize);

    GameObject* chunk = GetParent();
    chunk->_name = "Chunk_" + to_string(_chunkX) + "_" + to_string(_chunkY) + "_" + to_string(_chunkZ);
    chunk->GetTransform()->SetPosition(aabbMin.x(), aabbMin.y(), aabbMin.z());
    //chunk->SetFixedAABB(aabbMin, aabbMax);

    /** Allocate space for each layer **/

    int current_xSize = _xSize;
    int current_ySize = _ySize;
    int current_zSize = _zSize;

    this->_layers.resize(nbOfLayers);

    for (int layerID = 0; layerID < nbOfLayers; layerID++) {
        vector<vector<vector<unsigned char>>> layer;
        layer.resize(current_xSize);

        for (int x = 0; x < current_xSize; x++) {
            vector<vector<unsigned char>> layerX;
            layerX.resize(current_ySize);

            for (int y = 0; y < current_ySize; y++) {
                vector<unsigned char> layerXY;
                layerXY.resize(current_zSize);
                layerX[y] = layerXY;
            }

            layer[x] = layerX;
        }

        _layers[layerID] = layer;

        current_xSize /= _layerSizeReductionFactor;
        current_ySize /= _layerSizeReductionFactor;
        current_zSize /= _layerSizeReductionFactor;
    }
}

unsigned char TerrainComponent::getVoxelType(int x, int y, int z, int layerID) {
    int _currentLayerSizeReductionFactor = pow(_layerSizeReductionFactor, layerID);
    int xSize = _xSize / _currentLayerSizeReductionFactor;
    int ySize = _ySize / _currentLayerSizeReductionFactor;
    int zSize = _zSize / _currentLayerSizeReductionFactor;

    if (x < 0 || x >= xSize || y < 0 || y >= ySize || z < 0 || z >= zSize) {
        cerr << "Trying to get a voxel (" << x << ", " << y << ", " << z << ") outside of the defined layer (" << layerID << ")" << endl;
        return MaterialId::AIR;
    }

    return _layers[layerID][x][y][z];
}

/** Returns:
 *  -1 = error
 *   0 = voxel not modified
 *   1 = voxel modified
 **/
int TerrainComponent::setVoxelType(int x, int y, int z, unsigned char voxelMaterial, int layerID) {
    int _currentLayerSizeReductionFactor = pow(_layerSizeReductionFactor, layerID);
    int xSize = _xSize / _currentLayerSizeReductionFactor;
    int ySize = _ySize / _currentLayerSizeReductionFactor;
    int zSize = _zSize / _currentLayerSizeReductionFactor;

    if (x < 0 || x >= xSize || y < 0 || y >= ySize || z < 0 || z >= zSize) {
        cerr << "Trying to set a voxel (" << x << ", " << y << ", " << z << ") outside of the defined layer (" << layerID << ")" << endl;
        return -1;
    }

    _layers[0][x][y][z] = voxelMaterial;
    return 1;
}

int TerrainComponent::getNumberOfLayers() {
    return _layers.size();
}

vector<vector<vector<unsigned char>>> TerrainComponent::getLayer(int layerID) {
    return _layers[layerID];
}


/** DEBUG: print the layers from bottom-up **/
void TerrainComponent::debugPrintLayers() {
    cout << "TerrainComponent.debugPrintLayers() <-" << endl;
    int i = 0;
    for (auto layer : _layers) {
        cout << "##########     LAYER   " << i++ << "   ##########" << endl << endl;
        for (unsigned long long  y = 0; y < layer[0].size(); y++) {
            for (unsigned long long z = 0; z < layer[0][0].size(); z++) {
                for (unsigned long long x = 0; x < layer.size(); x++) {
                    cout << (int)layer[x][y][z] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << "TerrainComponent.debugPrintLayers() ->" << endl;
}

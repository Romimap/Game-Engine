#include "perlin2dterraincomponent.h"


Perlin2dTerrainComponent::Perlin2dTerrainComponent(int chunkX, int chunkY, int chunkZ, int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor,
                                                   const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence, GameObject* parent)
    : TerrainComponent(chunkX, chunkY, chunkZ, xSize, ySize, zSize, nbOfLayers, layerSizeReductionFactor, parent) {

    this->_name = "Perlin2dTerrainComponent";

    /** Generate terrain for the most detailed layer using perlin noise **/

    vector<vector<vector<unsigned char>>> &layer = _layers[0];

    float xFrequency = frequency / (float) _xSize;
    float zFrequency = frequency / (float) _zSize;

    float xStart = chunkX * frequency;
    float zStart = chunkZ * frequency;

    for (int x = 0; x < _xSize; x++) {
        float xNoise = xStart + x * xFrequency;

        for (int z = 0; z < _zSize; z++) {
            float zNoise = zStart + z * zFrequency;

            const double noise = perlin.octave2D_01(xNoise, zNoise, octaves, persistence);
            int groundLevel = (int)(noise * (_ySize - 1));

            // Place stone
            for (int y = 0; y < groundLevel - 2; y++) {
                layer[x][y][z] = MaterialId::STONE;
            }

            // Place 2 dirt and 1 grass
            if (groundLevel > 1) layer[x][groundLevel - 2][z] = MaterialId::DIRT;
            if (groundLevel > 0) layer[x][groundLevel - 1][z] = MaterialId::DIRT;
            layer[x][groundLevel][z] = MaterialId::GRASS;

            // Place air above
            for (int y = groundLevel + 1; y < _ySize; y++) {
                layer[x][y][z] = MaterialId::AIR;
            }
        }
    }

    /** Create upper (less detailed) layers **/

    for (int layerID = 0; layerID < nbOfLayers - 1; layerID++) {

        vector<vector<vector<unsigned char>>> &current_layer = _layers[layerID];
        vector<vector<vector<unsigned char>>> &upper_layer = _layers[layerID + 1];

        /** Update upper layer using current layer voxels **/

        int upper_xSize = upper_layer.size();
        int upper_ySize = upper_layer[0].size();
        int upper_zSize = upper_layer[0][0].size();

        // Use (x0, y0, z0) as coordinates in the upper layer
        for (int x0 = 0; x0 < upper_xSize; x0++) {
            int x1 = x0 * _layerSizeReductionFactor;

            for (int y0 = 0; y0 < upper_ySize; y0++) {
                int y1 = y0 * _layerSizeReductionFactor;

                for (int z0 = 0; z0 < upper_zSize; z0++) {
                    int z1 = z0 * _layerSizeReductionFactor;

                    /** Count current layer voxel types **/

                    int voxelTypeCount[MaterialId::MAX_ID + 1] = { 0 };

                    // Use (x1, y1, z1) as coordinates in the current layer
                    for (int i = 0; i < _layerSizeReductionFactor; i++) {
                        for (int j = 0; j < _layerSizeReductionFactor; j++) {
                            for (int k = 0; k < _layerSizeReductionFactor; k++) {
                                int voxelType = current_layer[x1 + i][y1 + j][z1 + k];
                                voxelTypeCount[voxelType]++;
                            }
                        }
                    }

                    /** Find the voxel type that occurs the most **/

                    int mostOccuringVoxel_id = 0;
                    int mostOccuringVoxel_count = 0;
                    for (unsigned int i = 0; i <= MaterialId::MAX_ID; i++) {
                        if (voxelTypeCount[i] > mostOccuringVoxel_count) {
                            mostOccuringVoxel_id = i;
                            mostOccuringVoxel_count = voxelTypeCount[i];
                        }
                    }

                    /** Set upper layer voxel type to the most curring voxel type found **/

                    upper_layer[x0][y0][z0] = (unsigned char) mostOccuringVoxel_id;
                }
            }
        }
    }

//    debugPrintLayers();
}

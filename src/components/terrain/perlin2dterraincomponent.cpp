#include "perlin2dterraincomponent.h"


Perlin2dTerrainComponent::Perlin2dTerrainComponent(int chunkX, int chunkZ, const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence, float roughness, GameObject* parent) : Component(parent) {

    this->_name = "Perlin2dTerrainComponent";
    _octreeComponent = parent->GetComponent<OctreeComponent>();

    /** Generate terrain for the most detailed layer using perlin noise **/

    std::vector<std::vector<std::vector<std::vector<unsigned char>>>> &layers = *_octreeComponent->getLayers();
    std::vector<std::vector<std::vector<unsigned char>>> &layer = layers[0];

    int xSize = _octreeComponent->getXSize();
    int ySize = _octreeComponent->getYSize();
    int zSize = _octreeComponent->getZSize();

    const double finalRoughness = roughness * (double)((xSize + zSize) / 2.0) / (double)ySize;

    float xFrequency = frequency / (float) xSize;
    float zFrequency = frequency / (float) zSize;

    float xStart = chunkX * frequency;
    float zStart = chunkZ * frequency;

    for (int x = 0; x < xSize; x++) {
        float xNoise = xStart + x * xFrequency;

        for (int z = 0; z < zSize; z++) {
            float zNoise = zStart + z * zFrequency;

            double noise = perlin.octave2D_01(xNoise, zNoise, octaves, persistence);

            // Flatten noise
            noise = 0.5 + (noise - 0.5) * finalRoughness;

            int groundLevel = (int)(noise * (ySize - 1));

            // Place stone
            for (int y = 0; y < groundLevel - 2; y++) {
                layer[x][y][z] = MaterialId::STONE;
            }

            // Place 2 dirt and 1 grass
            if (groundLevel > 1) layer[x][groundLevel - 2][z] = MaterialId::DIRT;
            if (groundLevel > 0) layer[x][groundLevel - 1][z] = MaterialId::DIRT;
            layer[x][groundLevel][z] = MaterialId::GRASS;

            // Place air above
            for (int y = groundLevel + 1; y < ySize; y++) {
                layer[x][y][z] = MaterialId::AIR;
            }
        }
    }

    /** Create upper (less detailed) layers **/

    int layerSizeReductionFactor = _octreeComponent->getLayerSizeReductionFactor();
    for (int layerID = 0; layerID < _octreeComponent->getNumberOfLayers() - 1; layerID++) {

        std::vector<std::vector<std::vector<unsigned char>>> &current_layer = layers[layerID];
        std::vector<std::vector<std::vector<unsigned char>>> &upper_layer = layers[layerID + 1];

        /** Update upper layer using current layer voxels **/

        int upper_xSize = upper_layer.size();
        int upper_ySize = upper_layer[0].size();
        int upper_zSize = upper_layer[0][0].size();

        // Use (x0, y0, z0) as coordinates in the upper layer
        for (int x0 = 0; x0 < upper_xSize; x0++) {
            int x1 = x0 * layerSizeReductionFactor;

            for (int y0 = 0; y0 < upper_ySize; y0++) {
                int y1 = y0 * layerSizeReductionFactor;

                for (int z0 = 0; z0 < upper_zSize; z0++) {
                    int z1 = z0 * layerSizeReductionFactor;

                    /** Count current layer voxel types **/

                    int voxelTypeCount[MaterialId::MAX_ID + 1] = { 0 };

                    // Use (x1, y1, z1) as coordinates in the current layer
                    for (int i = 0; i < layerSizeReductionFactor; i++) {
                        for (int j = 0; j < layerSizeReductionFactor; j++) {
                            for (int k = 0; k < layerSizeReductionFactor; k++) {
                                int voxelType = current_layer[x1 + i][y1 + j][z1 + k];
                                voxelTypeCount[voxelType]++;
                            }
                        }
                    }

                    /** Find the voxel type that occurs the most **/

                    int mostOccuringVoxel_id = 0;
                    int mostOccuringVoxel_count = 0;
                    for (unsigned int i = 1; i <= MaterialId::MAX_ID; i++) {
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
}

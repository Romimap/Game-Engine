#include "worldgeneratorcomponent.h"

WorldGeneratorComponent::WorldGeneratorComponent(string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent)
    : Component(parent) {

    this->_worldName = worldName;
    this->_terrainType = terrainType;
    this->_seed = seed;

    cout << "Generating world... (WorldGeneratorComponent)" << endl;

    auto start = chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{seed};

    int xMin = 0, xMax = 0;
    int yMin = 0, yMax = 0; // Don't touch until vertical chunks are implemented! y should always = 0
    int zMin = 0, zMax = 0;

    int chunksCount = 0;
    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            for (int z = zMin; z <= zMax; z++) {
                QVector3D aabbMin(x * _CHUNK_X_SIZE, y * _CHUNK_Y_SIZE, z * _CHUNK_Z_SIZE);
                QVector3D aabbMax((x + 1) * _CHUNK_X_SIZE, (y + 1) * _CHUNK_Y_SIZE, (z + 1) * _CHUNK_Z_SIZE);

                GameObject chunk(parent);
                chunk.GetTransform()->SetPosition(aabbMin.x(), aabbMin.y(), aabbMin.z());
                chunk.SetFixedAABB(aabbMin, aabbMax);

                Perlin2dTerrainComponent TC(0, 0, 0, _CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, _CHUNK_NB_OF_LAYERS, _CHUNK_LAYER_SIZE_REDUCTION_FACTOR, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE, GetParent());

                chunksCount++;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "World generation done (created " << chunksCount << " chunks) in " << elapsed.count() << "s (WorldGeneratorComponent)" << endl;
}

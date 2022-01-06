#include "worldgeneratorcomponent.h"


WorldGeneratorComponent::WorldGeneratorComponent(string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent)
    : Component(parent) {

    this->_worldName = worldName;
    this->_terrainType = terrainType;
    this->_seed = seed;

    string className = " (WorldGeneratorComponent)";

    cout << "Generating world..." << className << endl;

    auto start = chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{seed};

    int xMin = -TERRAIN_GEN_SQUARE_RADIUS, xMax = TERRAIN_GEN_SQUARE_RADIUS;
    int yMin = 0, yMax = 0; // Don't touch until vertical chunks are implemented! y should always = 0
    int zMin = -TERRAIN_GEN_SQUARE_RADIUS, zMax = TERRAIN_GEN_SQUARE_RADIUS;

    int chunksCount = 0;
    int totalChunks = (xMax - xMin + 1) * (yMax - yMin + 1) * (zMax - zMin + 1);

    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            for (int z = zMin; z <= zMax; z++) {
                GameObject* chunk = new GameObject(GetParent());
                Perlin2dTerrainComponent* TC = new Perlin2dTerrainComponent(x, y, z, _CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, _CHUNK_NB_OF_LAYERS, _CHUNK_LAYER_SIZE_REDUCTION_FACTOR, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE, chunk);

                chunksCount++;

                int percentage = (100 * chunksCount) / totalChunks;
                cout << "\rProgress: " << chunksCount << "/" << totalChunks << " chunks (" << percentage << "%)" << className << flush;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "World generation done (created " << chunksCount << " chunks) in " << elapsed.count() << "s" << className << endl;
}

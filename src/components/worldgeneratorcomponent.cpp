#include "worldgeneratorcomponent.h"

WorldGeneratorComponent::WorldGeneratorComponent(string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent)
    : Component(parent) {

    this->_worldName = worldName;
    this->_terrainType = terrainType;
    this->_seed = seed;

    cout << "Generating world... (WorldGeneratorComponent)" << endl;

    auto start = chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{seed};

    GameObject chunk(parent);
    Perlin2dTerrainComponent TC(0, 0, 0, _CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, _CHUNK_NB_OF_LAYERS, _CHUNK_LAYER_SIZE_REDUCTION_FACTOR, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE, parent);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "World generation done in " << elapsed.count() << "s (WorldGeneratorComponent)" << endl;
}

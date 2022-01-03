#include "worldgenerator.h"

WorldGenerator::WorldGenerator(string worldName, siv::PerlinNoise::seed_type seed) {
    auto start = chrono::high_resolution_clock::now();

    this->_worldName = worldName;
    this->_seed = seed;

    this->_perlin = siv::PerlinNoise{seed};

    Chunk chunk(0, 0, 0, _CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, 3, 4, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "World generation done in " << elapsed.count() << "s" << endl;
}

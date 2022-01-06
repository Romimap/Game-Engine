#include "worldgeneratorcomponent.h"


WorldGeneratorComponent::WorldGeneratorComponent(string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent)
    : Component(parent) {

    this->NAME = "WorldGeneratorComponent";
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
                Perlin2dTerrainComponent* TC;

                switch (_terrainType) {
                    case TerrainType::PERLIN_2D:
                        TC = new Perlin2dTerrainComponent(x, y, z, _CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, _CHUNK_NB_OF_LAYERS, _CHUNK_LAYER_SIZE_REDUCTION_FACTOR, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE, chunk);
                        break;
                    case TerrainType::PERLIN_3D:
                        cerr << "PERLIN_3D terrain generation not implemented yet, aborting world creation" << endl;
                        return;
                    case TerrainType::FLAT:
                        cerr << "FLAT terrain generation not implemented yet, aborting world creation" << endl;
                        return;
                    case TerrainType::FLOATING_ISLANDS:
                        cerr << "FLOATING_ISLANDS terrain generation not implemented yet, aborting world creation" << endl;
                        return;
                    case TerrainType::BIG_CAVES:
                        cerr << "BIG_CAVES terrain generation not implemented yet, aborting world creation" << endl;
                        return;
                    default:
                        cerr << "Unknown terrain generation specified, aborting world creation" << endl;
                        return;
                }

                chunksCount++;

                int percentage = (100 * chunksCount) / totalChunks;
                cout << "\rProgress: " << chunksCount << "/" << totalChunks << " chunks (" << percentage << "%)" << className << flush;
            }
        }
    }
    cout << endl;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "World generation done (created " << chunksCount << " chunks) in " << elapsed.count() << "s" << className << endl;
}

WorldGeneratorComponent::~WorldGeneratorComponent() {
    cout << "Call to WorldGeneratorComponent destructor" << endl;
}
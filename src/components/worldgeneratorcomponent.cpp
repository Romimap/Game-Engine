#include "worldgeneratorcomponent.h"


WorldGeneratorComponent::WorldGeneratorComponent(std::string worldName, TerrainType terrainType, siv::PerlinNoise::seed_type seed, GameObject* parent)
    : Component(parent) {

    this->_name = "WorldGeneratorComponent";
    this->_worldName = worldName;
    this->_terrainType = terrainType;
    this->_seed = seed;

    std::string className = " (WorldGeneratorComponent)";

    std::cout << "Generating world..." << className << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    this->_perlin = siv::PerlinNoise{seed};

    int xMin = -TERRAIN_GEN_SQUARE_RADIUS, xMax = TERRAIN_GEN_SQUARE_RADIUS;
    int yMin = 0, yMax = 0; // Don't touch until vertical chunks are implemented! y should always = 0
    int zMin = -TERRAIN_GEN_SQUARE_RADIUS, zMax = TERRAIN_GEN_SQUARE_RADIUS;

    int chunksCount = 0;
    int totalChunks = (xMax - xMin + 1) * (yMax - yMin + 1) * (zMax - zMin + 1);

    /** Create the chunks **/

    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            for (int z = zMin; z <= zMax; z++) {
                GameObject* chunk = new GameObject("Chunk", GetParent());
                chunk->GetTransform()->SetPosition(x * _CHUNK_X_SIZE, y * _CHUNK_Y_SIZE, z * _CHUNK_Z_SIZE);
                new OctreeComponent(_CHUNK_X_SIZE, _CHUNK_Y_SIZE, _CHUNK_Z_SIZE, _CHUNK_NB_OF_LAYERS, _CHUNK_LAYER_SIZE_REDUCTION_FACTOR, chunk);

                switch (_terrainType) {
                case TerrainType::PERLIN_2D:
                    new Perlin2dTerrainComponent(x, z, _perlin, _OCTAVES, _FREQUENCY, _PERSISTENCE, _ROUGHNESS, chunk);
                    break;
                case TerrainType::PERLIN_3D:
                    std::cerr << "PERLIN_3D terrain generation not implemented yet, aborting world creation" << std::endl;
                    return;
                case TerrainType::FLAT:
                    std::cerr << "FLAT terrain generation not implemented yet, aborting world creation" << std::endl;
                    return;
                case TerrainType::FLOATING_ISLANDS:
                    std::cerr << "FLOATING_ISLANDS terrain generation not implemented yet, aborting world creation" << std::endl;
                    return;
                case TerrainType::BIG_CAVES:
                    std::cerr << "BIG_CAVES terrain generation not implemented yet, aborting world creation" << std::endl;
                    return;
                default:
                    std::cerr << "Unknown terrain generation specified, aborting world creation" << std::endl;
                    return;
                }

                //NOTE: dont forget to uncomment
                //chunk->SetCollider(new OctreeCollider());

                new OctreeRendererComponent(chunk);

                chunksCount++;

                int percentage = (100 * chunksCount) / totalChunks;
                std::cout << "\rProgress: " << chunksCount << "/" << totalChunks << " chunks (" << percentage << "%)" << className << std::flush;
            }
        }
    }
    std::cout << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "World generation done (created " << chunksCount << " chunks) in " << elapsed.count() << "s" << className << std::endl;
}

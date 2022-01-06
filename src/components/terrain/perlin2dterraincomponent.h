#ifndef PERLIN2DTERRAINCOMPONENT_H
#define PERLIN2DTERRAINCOMPONENT_H


#include "src/gameobject.h"

#include "src/components/terrain/terraincomponent.h"


using namespace std;


class Perlin2dTerrainComponent : public TerrainComponent {
    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Perlin2dTerrainComponent(int chunkX, int chunkY, int chunkZ, int xSize, int ySize, int zSize, int nbOfLayers, int layerSizeReductionFactor,
                             const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence, GameObject* parent);
};

#endif // PERLIN2DTERRAINCOMPONENT_H

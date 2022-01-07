#ifndef PERLIN2DTERRAINCOMPONENT_H
#define PERLIN2DTERRAINCOMPONENT_H


#include "src/gameobject.h"

#include "src/components/terrain/octreecomponent.h"


using namespace std;


class Perlin2dTerrainComponent : Component {
    OctreeComponent* _octreeComponent;
    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Perlin2dTerrainComponent(int chunkX, int chunkZ,
                             const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence, float roughness, GameObject* parent);
};

#endif // PERLIN2DTERRAINCOMPONENT_H

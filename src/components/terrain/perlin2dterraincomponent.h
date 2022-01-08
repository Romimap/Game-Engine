#ifndef PERLIN2DTERRAINCOMPONENT_H
#define PERLIN2DTERRAINCOMPONENT_H


#include "src/gameobject.h"

#include "src/components/terrain/octreecomponent.h"


class Perlin2dTerrainComponent : Component {
    /*** ATTRIBUTES ***/
protected:
    OctreeComponent* _octreeComponent;

    /*** METHODS ***/
public:
    /** CONSTRUCTORS/DESTRUCTORS **/
    Perlin2dTerrainComponent(int chunkX, int chunkY, int chunkZ, const siv::PerlinNoise &perlin, int octaves, float frequency, float persistence, float roughness, GameObject* parent);
};

#endif // PERLIN2DTERRAINCOMPONENT_H

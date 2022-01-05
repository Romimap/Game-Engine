#ifndef CONSTS_H
#define CONSTS_H


namespace MaterialId {
    const unsigned int MAX_ID = 3;

    const unsigned char AIR = 0;
    const unsigned char STONE = 1;
    const unsigned char DIRT = 2;
    const unsigned char GRASS = 3;
}


enum TerrainType {
    PERLIN_2D,
    PERLIN_3D,
    FLAT = 2,
    FLOATING_ISLANDS,
    BIG_CAVES
};


#endif // CONSTS_H

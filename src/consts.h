#ifndef CONSTS_H
#define CONSTS_H


namespace MaterialId {
    const unsigned int MAX_ID = 3;

    const unsigned char AIR = 0;
    const unsigned char STONE = 1;
    const unsigned char DIRT = 2;
    const unsigned char GRASS = 3;
}


namespace TerrainType {
    const int PERLIN_2D = 0;
    const int PERLIN_3D = 1;
    const int FLAT = 2;
    const int FLOATING_ISLANDS = 3;
    const int BIG_CAVES = 4;
}


#endif // CONSTS_H

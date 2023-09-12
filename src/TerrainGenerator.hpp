//
// Created by Michal Walczyk on 08/09/2023.
//

#include "FastNoise.h"

#ifndef TILES_TERRAINGENERATOR_HPP
#define TILES_TERRAINGENERATOR_HPP


class TerrainGenerator {
private:
    FastNoise chunksNoise;
    FastNoise decalNoise;
    FastNoise* noises[2];
    int getDecalsValue(double value);
    int getBiomeValue(double value);
    int seed;
public:
    enum GenerationType{
        Biomes = 0,
        Decals = 1
    };
    enum BiomeTypes{
        Plains = 0,
        Forest = 1,
        Desert = 2
    };
    TerrainGenerator();
    int* getChunk(int x, int y);
    int* getChunk(int x, int y, GenerationType generationType);
    int getSeed() const;
    void setSeed(int seed);
};


#endif //TILES_TERRAINGENERATOR_HPP

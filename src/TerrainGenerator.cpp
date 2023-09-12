//
// Created by Michal Walczyk on 08/09/2023.
//

#include "TerrainGenerator.hpp"
#include "iostream"

TerrainGenerator::TerrainGenerator() {
    seed = 0;

    chunksNoise = FastNoise();
    chunksNoise.SetSeed(seed);
    chunksNoise.SetFrequency(0.001);
    chunksNoise.SetNoiseType(FastNoise::NoiseType::PerlinFractal);

    decalNoise = FastNoise();
    decalNoise.SetSeed(seed);
    decalNoise.SetFrequency(0.9);
    decalNoise.SetNoiseType(FastNoise::NoiseType::Perlin);

    noises[GenerationType::Biomes] = &chunksNoise;
    noises[GenerationType::Decals] = &decalNoise;

}

int TerrainGenerator::getSeed() const {
    return seed;
}

void TerrainGenerator::setSeed(int seed) {
    TerrainGenerator::seed = seed;
}

double interpolate(double a, double b, double t) {
    return a * (1 - t) + b * t;
}

int* TerrainGenerator::getChunk(int x, int y) {
    int* ret = new int[1024];

    for (int j = 0; j < 32; ++j) {
        for (int i = 0; i < 32; ++i) {
            double tl = chunksNoise.GetNoise(i + x * 32, j + y * 32);
            double tr = chunksNoise.GetNoise(i + 1 + x * 32, j + y * 32);

            double bl = chunksNoise.GetNoise(i + x * 32, j + 1 + y * 32);
            double br = chunksNoise.GetNoise(i + 1 + x * 32, j + 1 + y * 32);

            double top = interpolate(tl, tr, tl * 10 - floor(tl * 10));
            double bottom = interpolate(bl, br, bl * 10 - floor(bl * 10));
            double value = interpolate(top,bottom, top * 10 - floor(top * 10));

            //region Ids based around generated values
            if (value < 0.05) {
                ret[j * 32 + i] = 15;
            } else if (value < 0.15) {
                ret[j * 32 + i] = 11;
            }else if (value < 0.25) {
                ret[j * 32 + i] = 13;
            }else if (value < 0.3) {
                ret[j * 32 + i] = 8;
            } else if (value < 0.4) {
                ret[j * 32 + i] = 2;
            }else if (value < 0.5) {
                ret[j * 32 + i] = 4;
            }else if (value < 0.6) {
                ret[j * 32 + i] = 9;
            }else if (value < 0.7) {
                ret[j * 32 + i] = 10;
            }else if (value < 0.8) {
                ret[j * 32 + i] = 0;
            }else if (value < 0.9) {
                ret[j * 32 + i] = 15;
            }
            //endregion
        }
    }
    return ret;
}

int* TerrainGenerator::getChunk(int x, int y, GenerationType generationType) {
    int* ret = new int[1024];

    for (int j = 0; j < 32; ++j) {
        for (int i = 0; i < 32; ++i) {
            double tl = noises[generationType]->GetNoise(i + x * 32, j + y * 32);
            double tr = noises[generationType]->GetNoise(i + 1 + x * 32, j + y * 32);

            double bl = noises[generationType]->GetNoise(i + x * 32, j + 1 + y * 32);
            double br = noises[generationType]->GetNoise(i + 1 + x * 32, j + 1 + y * 32);

            double top = interpolate(tl, tr, tl * 10 - floor(tl * 10));
            double bottom = interpolate(bl, br, bl * 10 - floor(bl * 10));
            double value = interpolate(top,bottom, top * 10 - floor(top * 10));

            if(generationType == GenerationType::Biomes)
                ret[j * 32 + i] = TerrainGenerator::getBiomeValue(value);
            else if(generationType == GenerationType::Decals)
                ret[j * 32 + i] = TerrainGenerator::getDecalsValue(value);
        }
    }
    return ret;
}

int TerrainGenerator::getDecalsValue(double value){
    std::cout << value << std::endl;
    if (value < 0) {
        return 0;
    }else
    if (value < 0.03) {
        return 14;
    }else
    if (value < 0.5) {
        return 0;
    }else
    if (value < 0.9) {
        return 142;
    } else {
        return 0;
    }
}

int TerrainGenerator::getBiomeValue(double value){
    if (value < 0.05) {
        return 15;
    } else if (value < 0.15) {
        return 11;
    }else if (value < 0.25) {
        return 12;
    }else if (value < 0.3) {
        return 8;
    } else if (value < 0.4) {
        return 2;
    }else if (value < 0.5) {
        return 4;
    }else if (value < 0.6) {
        return 9;
    }else if (value < 0.7) {
        return 10;
    }else if (value < 0.8) {
        return 17;
    }else if (value < 0.9) {
        return 14;
    }else{
        return 0;
    }
}
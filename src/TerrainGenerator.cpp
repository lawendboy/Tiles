//
// Created by Michal Walczyk on 08/09/2023.
//

#include "TerrainGenerator.hpp"

int TerrainGenerator::getSeed() const {
    return seed;
}

void TerrainGenerator::setSeed(int seed) {
    TerrainGenerator::seed = seed;
}

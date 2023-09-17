//
// Created by Michal Walczyk on 12/09/2023.
//

#include "TerrainGenerator.hpp"

#ifndef TILES_LOGICALCHUNK_HPP
#define TILES_LOGICALCHUNK_HPP


class LogicalChunk {
public:
    TerrainGenerator::BiomeTypes biome = TerrainGenerator::BiomeTypes::Plains;

    LogicalChunk();
    LogicalChunk(TerrainGenerator::BiomeTypes biomeType);

    int layerA[1024]{};
    int layerB[1024]{};
    int layerC[1024]{};

    struct position{
        int x = 0;
        int y = 0;
    };


    ~LogicalChunk();
};


#endif //TILES_LOGICALCHUNK_HPP

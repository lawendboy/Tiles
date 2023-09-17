//
// Created by Michal Walczyk on 12/09/2023.
//
#include "iostream"
#include "LogicalChunk.hpp"

LogicalChunk::LogicalChunk() {
    for(int i = 0; i < 1024; i++){
        LogicalChunk::layerA[i] = 0;
        LogicalChunk::layerB[i] = 0;
        LogicalChunk::layerC[i] = 0;
    }
}
LogicalChunk::LogicalChunk(TerrainGenerator::BiomeTypes biomeType) {
    for(int i = 0; i < 1024; i++){
        LogicalChunk::layerA[i] = 0;
        LogicalChunk::layerB[i] = 0;
        LogicalChunk::layerC[i] = 0;
    }

    LogicalChunk::biome = biomeType;

}

LogicalChunk::~LogicalChunk() = default;
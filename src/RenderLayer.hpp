//
// Created by Michal Walczyk on 11/09/2023.
//

#include "Chunk.hpp"
#include "TerrainGenerator.hpp"
#include "Shader.hpp"

#ifndef TILES_RENDERLAYER_HPP
#define TILES_RENDERLAYER_HPP

class RenderLayer {
private:
    int chunkOffsetX;
    int chunkOffsetY;
    Chunk* chunks[25];
    TerrainGenerator* terrainGenerator;
    TerrainGenerator::GenerationType generationType;
public:
    RenderLayer(Chunk* inputChunk[], TerrainGenerator &terrainGenerator, TerrainGenerator::GenerationType generationType);
    void DrawLayer(Shader* shader);

    void ShiftChunksToLeft();
    void ShiftChunksToRight();
    void ShiftChunksToUp();
    void ShiftChunksToDown();
};


#endif //TILES_RENDERLAYER_HPP

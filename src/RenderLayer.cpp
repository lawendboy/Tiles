//
// Created by Michal Walczyk on 11/09/2023.
//

#include "RenderLayer.hpp"

void RenderLayer::DrawLayer(Shader* shader) {
    int counter = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            shader->SetUniform("chunkVector", glm::vec2(3.2f * j, -3.2f * i));
            chunks[counter]->Draw();
            counter++;
        }
    }
}

RenderLayer::RenderLayer(Chunk* inputChunk[], TerrainGenerator &terrainGenerator, TerrainGenerator::GenerationType generationType) {
    chunkOffsetX = 0;
    chunkOffsetY = 0;

    RenderLayer::generationType = generationType;

    RenderLayer::terrainGenerator = &terrainGenerator;
    for(int i = 0; i < 25; i++){
        RenderLayer::chunks[i] = inputChunk[i];
    }
}

void RenderLayer::ShiftChunksToLeft() {

    chunkOffsetX--;

    delete(chunks[0]);
    delete(chunks[5]);
    delete(chunks[10]);
    delete(chunks[15]);
    delete(chunks[20]);

    chunks[0] = chunks[1];
    chunks[5] = chunks[6];
    chunks[10] = chunks[11];
    chunks[15] = chunks[16];
    chunks[20] = chunks[21];

    chunks[1] = chunks[2];
    chunks[6] = chunks[7];
    chunks[11] = chunks[12];
    chunks[16] = chunks[17];
    chunks[21] = chunks[22];

    chunks[2] = chunks[3];
    chunks[7] = chunks[8];
    chunks[12] = chunks[13];
    chunks[17] = chunks[18];
    chunks[22] = chunks[23];

    chunks[3] = chunks[4];
    chunks[8] = chunks[9];
    chunks[13] = chunks[14];
    chunks[18] = chunks[19];
    chunks[23] = chunks[24];

    chunks[4] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[9] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 1 + chunkOffsetY, generationType));

    chunks[14] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 2 + chunkOffsetY, generationType));

    chunks[19] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 3 + chunkOffsetY, generationType));

    chunks[24] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 4 + chunkOffsetY, generationType));
}

void RenderLayer::ShiftChunksToRight(){

    chunkOffsetX++;

    delete(chunks[4]);
    delete(chunks[9]);
    delete(chunks[14]);
    delete(chunks[19]);
    delete(chunks[24]);

    chunks[4] = chunks[3];
    chunks[9] = chunks[8];
    chunks[14] = chunks[13];
    chunks[19] = chunks[18];
    chunks[24] = chunks[23];

    chunks[3] = chunks[2];
    chunks[8] = chunks[7];
    chunks[13] = chunks[12];
    chunks[18] = chunks[17];
    chunks[23] = chunks[22];

    chunks[2] = chunks[1];
    chunks[7] = chunks[6];
    chunks[12] = chunks[11];
    chunks[17] = chunks[16];
    chunks[22] = chunks[21];

    chunks[1] = chunks[0];
    chunks[6] = chunks[5];
    chunks[11] = chunks[10];
    chunks[16] = chunks[15];
    chunks[21] = chunks[20];


    chunks[0] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[5] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 1 + chunkOffsetY, generationType));

    chunks[10] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 2 + chunkOffsetY, generationType));

    chunks[15] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 3 + chunkOffsetY, generationType));

    chunks[20] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 4 + chunkOffsetY, generationType));
}

void RenderLayer::ShiftChunksToDown(){

    chunkOffsetY--;

    delete(chunks[24]);
    delete(chunks[23]);
    delete(chunks[22]);
    delete(chunks[21]);
    delete(chunks[20]);

    chunks[24] = chunks[19];
    chunks[23] = chunks[18];
    chunks[22] = chunks[17];
    chunks[21] = chunks[16];
    chunks[20] = chunks[15];

    chunks[19] = chunks[14];
    chunks[18] = chunks[13];
    chunks[17] = chunks[12];
    chunks[16] = chunks[11];
    chunks[15] = chunks[10];

    chunks[14] = chunks[9];
    chunks[13] = chunks[8];
    chunks[12] = chunks[7];
    chunks[11] = chunks[6];
    chunks[10] = chunks[5];

    chunks[9] = chunks[4];
    chunks[8] = chunks[3];
    chunks[7] = chunks[2];
    chunks[6] = chunks[1];
    chunks[5] = chunks[0];


    chunks[4] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[3] = new Chunk(terrainGenerator->getChunk(3 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[2] = new Chunk(terrainGenerator->getChunk(2 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[1] = new Chunk(terrainGenerator->getChunk(1 - chunkOffsetX, 0 + chunkOffsetY, generationType));

    chunks[0] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 0 + chunkOffsetY, generationType));

}

void RenderLayer::ShiftChunksToUp(){

    chunkOffsetY++;

    delete(chunks[0]);
    delete(chunks[1]);
    delete(chunks[2]);
    delete(chunks[3]);
    delete(chunks[4]);

    chunks[0] = chunks[5];
    chunks[1] = chunks[6];
    chunks[2] = chunks[7];
    chunks[3] = chunks[8];
    chunks[4] = chunks[9];

    chunks[5] = chunks[10];
    chunks[6] = chunks[11];
    chunks[7] = chunks[12];
    chunks[8] = chunks[13];
    chunks[9] = chunks[14];

    chunks[10] = chunks[15];
    chunks[11] = chunks[16];
    chunks[12] = chunks[17];
    chunks[13] = chunks[18];
    chunks[14] = chunks[19];

    chunks[15] = chunks[20];
    chunks[16] = chunks[21];
    chunks[17] = chunks[22];
    chunks[18] = chunks[23];
    chunks[19] = chunks[24];


    chunks[20] = new Chunk(terrainGenerator->getChunk(0 - chunkOffsetX, 4 + chunkOffsetY, generationType));

    chunks[21] = new Chunk(terrainGenerator->getChunk(1 - chunkOffsetX, 4 + chunkOffsetY, generationType));

    chunks[22] = new Chunk(terrainGenerator->getChunk(2 - chunkOffsetX, 4 + chunkOffsetY, generationType));

    chunks[23] = new Chunk(terrainGenerator->getChunk(3 - chunkOffsetX, 4 + chunkOffsetY, generationType));

    chunks[24] = new Chunk(terrainGenerator->getChunk(4 - chunkOffsetX, 4 + chunkOffsetY, generationType));
}

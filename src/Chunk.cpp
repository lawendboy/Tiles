//
// Created by Michal Walczyk on 04/09/2023.
//

#include "Chunk.hpp"
#include "iostream"
#include "glad/glad.h"

Chunk::Chunk(const int *indices) {

    for (int i = 0; i < 1024; ++i) {
        Chunk::indices[i] = indices[i];
    }
//
//    for(int i = 0; i < 32; ++i){
//        for(int j = 0; j < 32; ++j){
//            std::cout << Chunk::indices[i * 32 + j] << ' ';
//        }
//        std::cout << std::endl;
//    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(Chunk::vao);

    glBindBuffer(GL_ARRAY_BUFFER, Chunk::vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (Chunk::vertices), Chunk::vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), (void*)(2 * sizeof (float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, Chunk::instanceVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof (instances), instances, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Chunk::indices), Chunk::indices, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof (int), nullptr);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(2, 1);

}

Chunk::~Chunk() {
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Chunk::Draw() {
    glBindVertexArray(Chunk::vao);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, sizeof (Chunk::indices) / sizeof (int));
}
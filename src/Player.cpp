//
// Created by Michal Walczyk on 11/09/2023.
//

#include "iostream"
#include "glad/glad.h"
#include "Player.hpp"

Player::Player(const char* textureFilePath) {

    Player::playerTexture = new Texture(textureFilePath);

    glGenVertexArrays(1, &iconVAO);
    glGenBuffers(1, &iconVBO);

    glBindVertexArray(Player::iconVAO);

    glBindBuffer(GL_ARRAY_BUFFER, Player::iconVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (Player::iconVertices), iconVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof (float) * 2));
    glEnableVertexAttribArray(1);

}

Player::~Player() {
    glDeleteBuffers(1, &iconVBO);
    glDeleteVertexArrays(1, &iconVAO);
    delete(Player::playerTexture);
}
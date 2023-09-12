//
// Created by Michal Walczyk on 11/09/2023.
//

#include "glm.hpp"
#include "Texture.hpp"

#ifndef TILES_PLAYER_HPP
#define TILES_PLAYER_HPP

class Player {
private:
    float iconVertices[16] = {
            0.0f, 0.0f,  0.0f, 1.0f,
            0.1f, 0.0f,  1.0f, 1.0f,
            0.1f,  0.1f,  1.0f, 0.0f,
            0.0f,  0.1f,  0.0f, 0.0f
    };
    unsigned int iconVAO;
    unsigned int iconVBO;
    Texture* playerTexture;
    glm::vec2 globalPosition;
public:
    Player(const char* textureFilePath);
    void Draw();
    ~Player();
};


#endif //TILES_PLAYER_HPP

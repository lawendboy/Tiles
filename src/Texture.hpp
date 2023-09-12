//
// Created by Michal Walczyk on 02/09/2023.
//

#ifndef TILES_TEXTURE_HPP
#define TILES_TEXTURE_HPP


class Texture {
public:
    Texture(const char* fileName);
    void Bind();
    ~Texture();
private:
    unsigned int id;
    int width;
    int height;
    int nrChannels;
};


#endif //TILES_TEXTURE_HPP

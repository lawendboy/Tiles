//
// Created by Michal Walczyk on 04/09/2023.
//

#ifndef TILES_CHUNK_HPP
#define TILES_CHUNK_HPP


class Chunk {
private:
//    float vertices[16] = {
//        0.0f, 0.0f,  0.0f, 1.0f,
//        0.1f, 0.0f,  1.0f, 1.0f,
//        0.1f,  0.1f,  1.0f, 0.0f,
//        0.0f,  0.1f,  0.0f, 0.0f
//    };
    float vertices[16] = {
        0.0f, 0.0f,  0.0f, 1.0f,
        0.1f, 0.0f,  1.0f, 1.0f,
        0.1f,  0.1f,  1.0f, 0.0f,
        0.0f,  0.1f,  0.0f, 0.0f
    };
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int instanceVBO{};
    int indices[1024]{};
public:
    explicit Chunk(const int *indices);
    ~Chunk();
    void Draw();
};


#endif //TILES_CHUNK_HPP

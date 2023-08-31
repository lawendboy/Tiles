//
// Created by Michal Walczyk on 31/08/2023.
//

#include "glad/glad.h"
#include "iostream"

#ifndef TILES_SHADER_HPP
#define TILES_SHADER_HPP


class Shader {
public:
    unsigned int id;
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    void Use();
private:
    void checkCompileErrors(GLuint shader, std::string type);
};


#endif //TILES_SHADER_HPP

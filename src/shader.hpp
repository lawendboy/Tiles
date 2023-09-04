//
// Created by Michal Walczyk on 31/08/2023.
//

#include "glad/glad.h"
#include "iostream"
#include "map"
#include "glm.hpp"

#ifndef TILES_SHADER_HPP
#define TILES_SHADER_HPP

class Shader {
public:
    unsigned int id;
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    void Use();
    void AddUniform(std::string uniformName);
    void RemoveUniform(std::string uniformName);

    void SetUniform(std::string uniformName, int value);
    void SetUniform(std::string uniformName, float value);
    void SetUniform(std::string uniformName, glm::vec2 value);
    void SetUniform(std::string uniformName, glm::vec3 value);
    void SetUniform(std::string uniformName, glm::vec4 value);
    void SetUniform(std::string uniformName, glm::mat2 value);
    void SetUniform(std::string uniformName, glm::mat3 value);
    void SetUniform(std::string uniformName, glm::mat4 value);

    void setId(unsigned int id);

private:
    void checkCompileErrors(GLuint shader, std::string type);
    std::map<std::string, int> uniforms;
};


#endif //TILES_SHADER_HPP

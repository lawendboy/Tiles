//
// Created by Michal Walczyk on 31/08/2023.
//

#include "Shader.hpp"
#include "glad/glad.h"
#include "iostream"
#include "fstream"
#include "sstream"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(geometryPath != nullptr){
            gShaderFile.open(vertexPath);

            std::stringstream  gShaderStream;
            gShaderStream << gShaderFile.rdbuf();

            gShaderFile.close();

            geometryCode = gShaderStream.str();
        }

    }catch(std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }


    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    Shader::checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    Shader::checkCompileErrors(fragment, "FRAGMENT");

    unsigned int geometry;
    if(geometryPath != nullptr){
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &gShaderCode, nullptr);
        glCompileShader(fragment);
        Shader::checkCompileErrors(geometry, "GEOMETRY");
    }

    Shader::id = glCreateProgram();
    glAttachShader(Shader::id, vertex);
    glAttachShader(Shader::id, fragment);

    if(geometryPath != nullptr)
        glAttachShader(Shader::id, geometry);

    glLinkProgram(Shader::id);
    Shader::checkCompileErrors(Shader::id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath != nullptr)
        glDeleteShader(geometry);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR from " << type << "\n" << infoLog << std::endl;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR from " << type << "\n" << infoLog << std::endl;
        }
    }
}

void Shader::Use() {
    glUseProgram(Shader::id);
}

void Shader::AddUniform(std::string uniformName) {
    int uniformLocation = glGetUniformLocation(Shader::id, uniformName.c_str());

    if(uniformLocation == -1){
        std::cerr << "ERROR::SHADER_UNIFORM did not found uniform: " << uniformName << std::endl;
    }

    Shader::uniforms[uniformName] = uniformLocation;
}

void Shader::SetUniform(std::string uniformName, glm::mat4 value) {
    glUniformMatrix4fv(Shader::uniforms[uniformName], 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(std::string uniformName, glm::vec2 value) {
    glUniform2fv(Shader::uniforms[uniformName], 1, glm::value_ptr(value));
}

void Shader::SetUniform(std::string uniformName, int value) {
    glUniform1i(Shader::uniforms[uniformName], value);
}


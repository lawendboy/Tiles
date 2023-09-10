cmake_minimum_required(VERSION 3.23)
project(Tiles)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_executable(Tiles src/main.cpp libs/glad/src/glad.c src/Shader.cpp src/Shader.hpp src/Texture.cpp src/Texture.hpp libs/FastNoise/FastNoise.cpp src/Chunk.cpp src/Chunk.hpp src/TerrainGenerator.cpp src/TerrainGenerator.hpp)

target_include_directories(Tiles PRIVATE src ${OPENGL_INCLUDE_DIR} libs/glad/include libs/glfw/include libs/glm libs/stb_image libs/FastNoise)
target_link_directories(Tiles PRIVATE libs/glfw)
target_link_libraries(Tiles PRIVATE glfw.3 ${OPENGL_LIBRARIES})

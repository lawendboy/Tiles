cmake_minimum_required(VERSION 3.23)
project(Tiles)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_executable(Tiles src/main.cpp libs/glad/src/glad.c src/shader.cpp src/shader.hpp src/texture.cpp src/texture.hpp libs/FastNoise/FastNoise.cpp src/chunk.cpp src/chunk.hpp)

target_include_directories(Tiles PRIVATE src ${OPENGL_INCLUDE_DIR} libs/glad/include libs/glfw/include libs/glm libs/stb_image libs/FastNoise)
target_link_directories(Tiles PRIVATE libs/glfw)
target_link_libraries(Tiles PRIVATE glfw.3 ${OPENGL_LIBRARIES})

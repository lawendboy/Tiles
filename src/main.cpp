#include "iostream"
#include "random"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "FastNoise.h"
#include "ext/matrix_transform.hpp"

#include "shader.hpp"
#include "texture.hpp";

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Tile engine"

int main() {

    /*
     * Ładowanie glfw
     */

    if(!glfwInit()){
        std::cerr << "Failed: glfwInit()\n";
        return -1;
    }

    GLFWwindow* window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, false);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if(window == nullptr){
        std::cerr << "Failed to create glfw window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    /*
     * Ładowanie glad
     */
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Failed to load glad\n";
    }

    glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);

    Shader mainShader("vertex.glsl", "fragment.glsl");

    mainShader.Use();

    Texture atlas("atlas.png");


    FastNoise myNoise;
    myNoise.SetSeed(48329892);          // Set a random seed
    myNoise.SetFrequency(0.05f);   // Adjust the frequency of the noise
    myNoise.SetInterp(FastNoise::Hermite); // Use Hermite interpolation for smoother noise

    const int width = 32;  // Width of the noise map
    const int height = 32; // Height of the noise map

    // Generate Perlin noise and print it
    int noiseVector[1024];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            noiseVector[y * height + x] = roundf(myNoise.GetNoise(x, y) * 10 + 10);
        }
    }

//    for(int i = 0; i < 32; i++){
//        for(int j = 0; j < 32; j++){
//            std::cout << noiseVector[i * 32 + j] << ' ';
//        }
//        std::cout << std::endl;
//    }

    int instances[] = {
            1,0, 10, 10, 10, 10, 10, 9,
            8, 8, 8, 8, 8, 9, 8, 8,
            6, 6, 6, 7, 7, 7, 8, 8,
            5, 5, 5, 5, 6, 6, 7, 7,
            4, 4, 4, 5, 5, 6, 6, 7,
            3, 3, 4, 4, 5, 5, 6, 6,
            3, 3, 4, 4, 4, 5, 5, 6,
            3, 3, 4, 4, 4, 4, 5, 5,

    };

    float vertices[] = {
            -0.125f, -0.125f,  0.0f, 1.0f,
            0.125f, -0.125f,  1.0f, 1.0f,
            0.125f,  0.125f,  1.0f, 0.0f,
            -0.125f,  0.125f,  0.0f, 0.0f
    };

    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), (void*)(2 * sizeof (float)));
    glEnableVertexAttribArray(1);

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof (instances), instances, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(noiseVector), noiseVector, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof (int), nullptr);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(2, 1);


    glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
    glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
    projectionMatrix = glm::scale(projectionMatrix, glm::vec3(0.1f));


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    mainShader.AddUniform("textureSampler");
    mainShader.SetUniform("textureSampler", 0);

    mainShader.AddUniform("viewMatrix");
    mainShader.SetUniform("viewMatrix", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    atlas.Bind();

    glClearColor(0.4431372549f, 0.6705882353f, 0.6392156863f, 1.0f);

    while(!glfwWindowShouldClose(window)){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, sizeof(instances) / sizeof (int));
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, sizeof(noiseVector) / sizeof (int));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
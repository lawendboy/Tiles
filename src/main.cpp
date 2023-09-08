#include "iostream"
#include "random"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "FastNoise.h"
#include "ext/matrix_transform.hpp"

#include "shader.hpp"
#include "texture.hpp"
#include "chunk.hpp"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Tile engine"

FastNoise myNoise;

const int genWidth = 32;  // Width of the noise map
const int genHeight = 32; // Height of the noise map

float zoom = 0.1;

Chunk* chunks[36];

int noiseVector[1024];

double interpolate(double a, double b, double t) {
    // Linear interpolation
    return a * (1 - t) + b * t;
}

int mapWidth = 32;
int mapHeight = 32;

void GenerateChunk(int offsetX, int offsetY){
    for (int y = 0; y < genHeight; ++y) {
        for (int x = 0; x < genWidth; ++x) {
            double tl = myNoise.GetNoise(x + offsetX * 32, y + offsetY * 32);
            double tr = myNoise.GetNoise(x + 1 + offsetX * 32, y + offsetY * 32);

            double bl = myNoise.GetNoise(x + offsetX * 32, y + 1 + offsetY * 32);
            double br = myNoise.GetNoise(x + 1 + offsetX * 32, y + 1 + offsetY * 32);

            double top = interpolate(tl, tr, tl * 10 - floor(tl * 10));
            double bottom = interpolate(bl, br, bl * 10 - floor(bl * 10));
            double value = interpolate(top,bottom, top * 10 - floor(top * 10));
//            std::cout << tl << ' ' << tr << ' ' << bl << ' ' << br << ' ' << top << ' ' << bottom << ' ' << value;
            if (value < 0.05) {
                noiseVector[y * genHeight + x] = 15;
            } else if (value < 0.1) {
                noiseVector[y * genHeight + x] = 11;
            }else if (value < 0.2) {
                noiseVector[y * genHeight + x] = 13;
            }else if (value < 0.3) {
                noiseVector[y * genHeight + x] = 6;
            } else if (value < 0.4) {
                noiseVector[y * genHeight + x] = 2;
            }else if (value < 0.5) {
                noiseVector[y * genHeight + x] = 4;
            }else if (value < 0.6) {
                noiseVector[y * genHeight + x] = 9;
            }else if (value < 0.7) {
                noiseVector[y * genHeight + x] = 10;
            }else if (value < 0.8) {
                noiseVector[y * genHeight + x] = 0;
            }else if (value < 0.9) {
                noiseVector[y * genHeight + x] = 15;
            }
        }
    }
}

int chunkOffsetX = 0;
int chunkOffsetY = 0;

double playerPositionX = 0.0;
double playerPositionY = 0.0;
glm::vec2 playerPositionVector;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Shader* mainShader;

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

    glfwSetKeyCallback(window, key_callback);



    /*
     * Ładowanie glad
     */
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Failed to load glad\n";
    }

    glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);

    mainShader = new Shader("vertex.glsl", "fragment.glsl");

    mainShader->Use();

    Texture atlas("atlas2.png");


    myNoise.SetSeed(10);
    myNoise.SetFrequency(0.01f);
    myNoise.SetInterp(FastNoise::Hermite);
    myNoise.SetNoiseType(FastNoise::Perlin);


    int counterChunks = 0;
    for(int i = 0 ; i < 6; i++){
        for(int j = 0; j < 6; j++){
            GenerateChunk(j, i);
//            generateBiomeMap(j, i);
            chunks[counterChunks++] = new Chunk(noiseVector);
        }
    }

    glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
    glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
    projectionMatrix = glm::scale(projectionMatrix, glm::vec3(0.5));
    projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-9.5, 9,0));


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    mainShader->AddUniform("textureSampler");
    mainShader->SetUniform("textureSampler", 0);

    mainShader->AddUniform("viewMatrix");
    mainShader->SetUniform("viewMatrix", projectionMatrix);

    mainShader->AddUniform("offsetVector");

//    mainShader.AddUniform("playerOffset");


    glActiveTexture(GL_TEXTURE0);
    atlas.Bind();

    glClearColor(0.4431372549f, 0.6705882353f, 0.6392156863f, 1.0f);

    glm::vec2 positionMatrix(1.0f);

    double lastTime = glfwGetTime();
    double currentTime;
    double deltaTime;

    int counter;
    while(!glfwWindowShouldClose(window)){

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        playerPositionVector.x += deltaTime * playerPositionX;
        playerPositionVector.y += deltaTime * playerPositionY;

//        if(playerPositionX >= )

        counter = 0;

        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                positionMatrix = glm::vec2(3.2f * j, -3.2f * i) + playerPositionVector;
                mainShader->SetUniform("offsetVector", positionMatrix);
                chunks[counter]->Draw();
                counter++;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void regenerateChunk(){
    int counterChunks = 0;
    for(int i = 0 ; i < 6; i++){
        for(int j = 0; j < 6; j++){
            GenerateChunk(j - chunkOffsetX, i - chunkOffsetY);
            delete(chunks[counterChunks]);
            chunks[counterChunks++] = new Chunk(noiseVector);
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        myNoise.SetSeed(myNoise.GetSeed() + 1);
        regenerateChunk();
    }

    if(key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS){

        zoom -= 0.1;

        glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
        glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
        projectionMatrix = glm::scale(projectionMatrix, glm::vec3(zoom));
        projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-9.5, 9,0));

        mainShader->SetUniform("viewMatrix", projectionMatrix);
    }

    if(key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS){

        zoom += 0.1;

        glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
        glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
        projectionMatrix = glm::scale(projectionMatrix, glm::vec3(zoom));
        projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-9.5, 9,0));

        mainShader->SetUniform("viewMatrix", projectionMatrix);
    }

    if(key == GLFW_KEY_UP && action == GLFW_PRESS){
        chunkOffsetY++;
        regenerateChunk();
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        chunkOffsetY--;
        regenerateChunk();
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        chunkOffsetX++;
        regenerateChunk();
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        chunkOffsetX--;
        regenerateChunk();
    }

    if(key == GLFW_KEY_W && action == GLFW_PRESS){
        playerPositionY = -1;
    }
    if(key == GLFW_KEY_W && action == GLFW_RELEASE){
        playerPositionY = 0;
    }

    if(key == GLFW_KEY_S && action == GLFW_PRESS){
        playerPositionY = 1;
    }
    if(key == GLFW_KEY_S && action == GLFW_RELEASE){
        playerPositionY = 0;
    }

    if(key == GLFW_KEY_A && action == GLFW_PRESS){
        playerPositionX = 1;
    }
    if(key == GLFW_KEY_A && action == GLFW_RELEASE){
        playerPositionX = 0;
    }

    if(key == GLFW_KEY_D && action == GLFW_PRESS){
        playerPositionX = -1;
    }
    if(key == GLFW_KEY_D && action == GLFW_RELEASE){
        playerPositionX = 0;
    }
}
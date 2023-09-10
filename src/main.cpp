#include "iostream"
#include "random"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "FastNoise.h"
#include "ext/matrix_transform.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "Chunk.hpp"
#include "TerrainGenerator.hpp"

#define WINDOW_WIDTH 1240
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Tile engine"

FastNoise myNoise;

const int genWidth = 32;  // Width of the noise map
const int genHeight = 32; // Height of the noise map
const int chunksWidth = 5;
const int chunksHeight = 5;

float zoom = 0.05;

Chunk* chunks[25];

int noiseVector[1024];

float moveSpeed = 1.0f;

double interpolate(double a, double b, double t) {
    // Linear interpolation
    return a * (1 - t) + b * t;
}

int mapWidth = 32;
int mapHeight = 32;

void GenerateChunk(int offsetX, int offsetY){
//
//    for(int i = 0; i < 1024; i++){
//        noiseVector[i] = offsetX + offsetY * 5;
//    }
//    std::cout << offsetX << ' ' << offsetY << std::endl;
///*
    for (int y = 0; y < genHeight; ++y) {
        for (int x = 0; x < genWidth; ++x) {
            double tl = myNoise.GetNoise(x + offsetX * 32, y + offsetY * 32);
            double tr = myNoise.GetNoise(x + 1 + offsetX * 32, y + offsetY * 32);

            double bl = myNoise.GetNoise(x + offsetX * 32, y + 1 + offsetY * 32);
            double br = myNoise.GetNoise(x + 1 + offsetX * 32, y + 1 + offsetY * 32);

            double top = interpolate(tl, tr, tl * 10 - floor(tl * 10));
            double bottom = interpolate(bl, br, bl * 10 - floor(bl * 10));
            double value = interpolate(top,bottom, top * 10 - floor(top * 10));

            //region Ids based around generated values
            if (value < 0.05) {
                noiseVector[y * genHeight + x] = 15;
            } else if (value < 0.15) {
                noiseVector[y * genHeight + x] = 11;
            }else if (value < 0.25) {
                noiseVector[y * genHeight + x] = 13;
            }else if (value < 0.3) {
                noiseVector[y * genHeight + x] = 8;
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
            //endregion
        }
    }
//    */
}

int chunkOffsetX = 0;
int chunkOffsetY = 0;

double playerPositionX = 0.0;
double playerPositionY = 0.0;
glm::vec2 playerPositionVector;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Shader* mainShader;

void regenerateChunk(){
    int counterChunks = 0;
    for(int i = 0 ; i < chunksHeight; i++){
        for(int j = 0; j < chunksWidth; j++){
            GenerateChunk(j - chunkOffsetX, i - chunkOffsetY);
//            std::cout << j << ' ' << chunkOffsetX << ' ' << i << ' ' << chunkOffsetY << ' ' << counterChunks << std::endl;
            delete(chunks[counterChunks]);
            chunks[counterChunks++] = new Chunk(noiseVector);
        }
    }
}

void ShiftChunksToDown(){

    chunkOffsetY--;

    delete(chunks[24]);
    delete(chunks[23]);
    delete(chunks[22]);
    delete(chunks[21]);
    delete(chunks[20]);

    chunks[24] = chunks[19];
    chunks[23] = chunks[18];
    chunks[22] = chunks[17];
    chunks[21] = chunks[16];
    chunks[20] = chunks[15];

    chunks[19] = chunks[14];
    chunks[18] = chunks[13];
    chunks[17] = chunks[12];
    chunks[16] = chunks[11];
    chunks[15] = chunks[10];

    chunks[14] = chunks[9];
    chunks[13] = chunks[8];
    chunks[12] = chunks[7];
    chunks[11] = chunks[6];
    chunks[10] = chunks[5];

    chunks[9] = chunks[4];
    chunks[8] = chunks[3];
    chunks[7] = chunks[2];
    chunks[6] = chunks[1];
    chunks[5] = chunks[0];

    GenerateChunk(4 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[4] = new Chunk(noiseVector);

    GenerateChunk(3 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[3] = new Chunk(noiseVector);

    GenerateChunk(2 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[2] = new Chunk(noiseVector);

    GenerateChunk(1 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[1] = new Chunk(noiseVector);

    GenerateChunk(0 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[0] = new Chunk(noiseVector);

}

void ShiftChunksToUp(){

    chunkOffsetY++;

    delete(chunks[0]);
    delete(chunks[1]);
    delete(chunks[2]);
    delete(chunks[3]);
    delete(chunks[4]);

    chunks[0] = chunks[5];
    chunks[1] = chunks[6];
    chunks[2] = chunks[7];
    chunks[3] = chunks[8];
    chunks[4] = chunks[9];

    chunks[5] = chunks[10];
    chunks[6] = chunks[11];
    chunks[7] = chunks[12];
    chunks[8] = chunks[13];
    chunks[9] = chunks[14];

    chunks[10] = chunks[15];
    chunks[11] = chunks[16];
    chunks[12] = chunks[17];
    chunks[13] = chunks[18];
    chunks[14] = chunks[19];

    chunks[15] = chunks[20];
    chunks[16] = chunks[21];
    chunks[17] = chunks[22];
    chunks[18] = chunks[23];
    chunks[19] = chunks[24];

    GenerateChunk(0 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[20] = new Chunk(noiseVector);

    GenerateChunk(1 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[21] = new Chunk(noiseVector);

    GenerateChunk(2 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[22] = new Chunk(noiseVector);

    GenerateChunk(3 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[23] = new Chunk(noiseVector);

    GenerateChunk(4 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[24] = new Chunk(noiseVector);
}

void ShiftChunksToLeft(){

    chunkOffsetX--;

    delete(chunks[0]);
    delete(chunks[5]);
    delete(chunks[10]);
    delete(chunks[15]);
    delete(chunks[20]);

    chunks[0] = chunks[1];
    chunks[5] = chunks[6];
    chunks[10] = chunks[11];
    chunks[15] = chunks[16];
    chunks[20] = chunks[21];

    chunks[1] = chunks[2];
    chunks[6] = chunks[7];
    chunks[11] = chunks[12];
    chunks[16] = chunks[17];
    chunks[21] = chunks[22];

    chunks[2] = chunks[3];
    chunks[7] = chunks[8];
    chunks[12] = chunks[13];
    chunks[17] = chunks[18];
    chunks[22] = chunks[23];

    chunks[3] = chunks[4];
    chunks[8] = chunks[9];
    chunks[13] = chunks[14];
    chunks[18] = chunks[19];
    chunks[23] = chunks[24];

    GenerateChunk(4 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[4] = new Chunk(noiseVector);

    GenerateChunk(4 - chunkOffsetX, 1 + chunkOffsetY);
    chunks[9] = new Chunk(noiseVector);

    GenerateChunk(4 - chunkOffsetX, 2 + chunkOffsetY);
    chunks[14] = new Chunk(noiseVector);

    GenerateChunk(4 - chunkOffsetX, 3 + chunkOffsetY);
    chunks[19] = new Chunk(noiseVector);

    GenerateChunk(4 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[24] = new Chunk(noiseVector);
}

void ShiftChunksToRight(){

    chunkOffsetX++;

    delete(chunks[4]);
    delete(chunks[9]);
    delete(chunks[14]);
    delete(chunks[19]);
    delete(chunks[24]);

    chunks[4] = chunks[3];
    chunks[9] = chunks[8];
    chunks[14] = chunks[13];
    chunks[19] = chunks[18];
    chunks[24] = chunks[23];

    chunks[3] = chunks[2];
    chunks[8] = chunks[7];
    chunks[13] = chunks[12];
    chunks[18] = chunks[17];
    chunks[23] = chunks[22];

    chunks[2] = chunks[1];
    chunks[7] = chunks[6];
    chunks[12] = chunks[11];
    chunks[17] = chunks[16];
    chunks[22] = chunks[21];

    chunks[1] = chunks[0];
    chunks[6] = chunks[5];
    chunks[11] = chunks[10];
    chunks[16] = chunks[15];
    chunks[21] = chunks[20];

    GenerateChunk(0 - chunkOffsetX, 0 + chunkOffsetY);
    chunks[0] = new Chunk(noiseVector);

    GenerateChunk(0 - chunkOffsetX, 1 + chunkOffsetY);
    chunks[5] = new Chunk(noiseVector);

    GenerateChunk(0 - chunkOffsetX, 2 + chunkOffsetY);
    chunks[10] = new Chunk(noiseVector);

    GenerateChunk(0 - chunkOffsetX, 3 + chunkOffsetY);
    chunks[15] = new Chunk(noiseVector);

    GenerateChunk(0 - chunkOffsetX, 4 + chunkOffsetY);
    chunks[20] = new Chunk(noiseVector);
}

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
    glfwWindowHint(GLFW_SAMPLES, 4); // Multisampling
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

    if(glfwExtensionSupported("GL_ARB_multisample")){
        glEnable(GL_MULTISAMPLE);
    }else{
        std::cerr << "Multisampling not supported" << std::endl;
    }

    mainShader = new Shader("vertex.glsl", "fragment.glsl");

    mainShader->Use();

//    Texture atlas("atlas2.png"); //33333333333333333333333333333333333333333333333333
    Texture atlas("atlas3.png"); //33333333333333333333333333333333333333333333333333


    myNoise.SetSeed(10);
    myNoise.SetFrequency(0.01f);
//    myNoise.SetInterp(FastNoise::Hermite);
    myNoise.SetNoiseType(FastNoise::Perlin);


    int counterChunks = 0;
    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            GenerateChunk(j, i);
            chunks[counterChunks++] = new Chunk(noiseVector);
        }
    }

    glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
    glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
    projectionMatrix = glm::scale(projectionMatrix, glm::vec3(0.3));
    projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-8, 8,0));


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    mainShader->AddUniform("textureSampler");
    mainShader->SetUniform("textureSampler", 0);

    mainShader->AddUniform("viewMatrix");
    mainShader->SetUniform("viewMatrix", projectionMatrix);

    mainShader->AddUniform("offsetVector");

    glActiveTexture(GL_TEXTURE0);
    atlas.Bind();

//    glClearColor(0.4431372549f, 0.6705882353f, 0.6392156863f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

        playerPositionVector.x += deltaTime * playerPositionX * moveSpeed;
        playerPositionVector.y += deltaTime * playerPositionY * moveSpeed;

        if(playerPositionVector.x >= 3.2){
            playerPositionVector.x = 0;
            ShiftChunksToRight();
        }else if(playerPositionVector.x <= -3.2){
            playerPositionVector.x = 0;
            ShiftChunksToLeft();
        }else if(playerPositionVector.y >= 3.2){
            playerPositionVector.y = 0;
            ShiftChunksToUp();
        }else if(playerPositionVector.y <= -3.2){
            playerPositionVector.y = 0;
            ShiftChunksToDown();
        }

//        std::cout << playerPositionVector.x << ' ' << playerPositionVector.y << std::endl;

        counter = 0;

        for(int i = 0; i < chunksHeight; i++){
            for(int j = 0; j < chunksWidth; j++){
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

//    if(key == GLFW_KEY_P && action == GLFW_PRESS){}

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        myNoise.SetSeed(myNoise.GetSeed() + 1);
        regenerateChunk();
    }

    if(key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS){

        zoom -= 0.1;
        if(zoom <= 0.1)
            zoom = 0.1;

        glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
        glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
        projectionMatrix = glm::scale(projectionMatrix, glm::vec3(zoom));
        projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-8, 8,0));

        mainShader->SetUniform("viewMatrix", projectionMatrix);
    }

    if(key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS){

        zoom += 0.1;

        glm::vec3 projectionVec = glm::vec3 (1.0f, (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.0f);
        glm::mat4 projectionMatrix = glm::scale(glm::mat4(1.0f), projectionVec);
        projectionMatrix = glm::scale(projectionMatrix, glm::vec3(zoom));
        projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-8, 8,0));

        mainShader->SetUniform("viewMatrix", projectionMatrix);
    }

    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
        moveSpeed = 2.0f;
    }

    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
        moveSpeed = 1.0f;
    }

    if(key == GLFW_KEY_UP && action == GLFW_PRESS){
        ShiftChunksToUp();
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        ShiftChunksToDown();
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        ShiftChunksToLeft();
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        ShiftChunksToRight();
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
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
#include "Player.hpp"
#include "RenderLayer.hpp"

#define WINDOW_WIDTH 1240
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Tile engine"

const int genWidth = 32;  // Width of the noise map
const int genHeight = 32; // Height of the noise map
const int chunksWidth = 5;
const int chunksHeight = 5;

float zoom = 0.05;

Chunk* chunks[25];
Chunk* chunks2[25];

int noiseVector[1024];

float moveSpeed = 1.0f;

int chunkOffsetX = 0;
int chunkOffsetY = 0;

double playerPositionX = 0.0;
double playerPositionY = 0.0;
glm::vec2 playerPositionVector;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Shader* mainShader;

RenderLayer* firstLayer;
RenderLayer* secondLayer;

//
//void regenerateChunk(){
//    int counterChunks = 0;
//    for(int i = 0 ; i < chunksHeight; i++){
//        for(int j = 0; j < chunksWidth; j++){
//            GenerateChunk(j - chunkOffsetX, i - chunkOffsetY);
//            delete(chunks[counterChunks]);
//            chunks[counterChunks++] = new Chunk(noiseVector);
//        }
//    }
//}
//
//void ShiftChunksToDown(){
//
//    chunkOffsetY--;
//
//    delete(chunks[24]);
//    delete(chunks[23]);
//    delete(chunks[22]);
//    delete(chunks[21]);
//    delete(chunks[20]);
//
//    chunks[24] = chunks[19];
//    chunks[23] = chunks[18];
//    chunks[22] = chunks[17];
//    chunks[21] = chunks[16];
//    chunks[20] = chunks[15];
//
//    chunks[19] = chunks[14];
//    chunks[18] = chunks[13];
//    chunks[17] = chunks[12];
//    chunks[16] = chunks[11];
//    chunks[15] = chunks[10];
//
//    chunks[14] = chunks[9];
//    chunks[13] = chunks[8];
//    chunks[12] = chunks[7];
//    chunks[11] = chunks[6];
//    chunks[10] = chunks[5];
//
//    chunks[9] = chunks[4];
//    chunks[8] = chunks[3];
//    chunks[7] = chunks[2];
//    chunks[6] = chunks[1];
//    chunks[5] = chunks[0];
//
//    GenerateChunk(4 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[4] = new Chunk(noiseVector);
//
//    GenerateChunk(3 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[3] = new Chunk(noiseVector);
//
//    GenerateChunk(2 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[2] = new Chunk(noiseVector);
//
//    GenerateChunk(1 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[1] = new Chunk(noiseVector);
//
//    GenerateChunk(0 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[0] = new Chunk(noiseVector);
//
//}
//
//void ShiftChunksToUp(){
//
//    chunkOffsetY++;
//
//    delete(chunks[0]);
//    delete(chunks[1]);
//    delete(chunks[2]);
//    delete(chunks[3]);
//    delete(chunks[4]);
//
//    chunks[0] = chunks[5];
//    chunks[1] = chunks[6];
//    chunks[2] = chunks[7];
//    chunks[3] = chunks[8];
//    chunks[4] = chunks[9];
//
//    chunks[5] = chunks[10];
//    chunks[6] = chunks[11];
//    chunks[7] = chunks[12];
//    chunks[8] = chunks[13];
//    chunks[9] = chunks[14];
//
//    chunks[10] = chunks[15];
//    chunks[11] = chunks[16];
//    chunks[12] = chunks[17];
//    chunks[13] = chunks[18];
//    chunks[14] = chunks[19];
//
//    chunks[15] = chunks[20];
//    chunks[16] = chunks[21];
//    chunks[17] = chunks[22];
//    chunks[18] = chunks[23];
//    chunks[19] = chunks[24];
//
//    GenerateChunk(0 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[20] = new Chunk(noiseVector);
//
//    GenerateChunk(1 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[21] = new Chunk(noiseVector);
//
//    GenerateChunk(2 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[22] = new Chunk(noiseVector);
//
//    GenerateChunk(3 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[23] = new Chunk(noiseVector);
//
//    GenerateChunk(4 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[24] = new Chunk(noiseVector);
//}
//
//void ShiftChunksToLeft() {
//
//    chunkOffsetX--;
//
//    delete (chunks[0]);
//    delete (chunks[5]);
//    delete (chunks[10]);
//    delete (chunks[15]);
//    delete (chunks[20]);
//
//    chunks[0] = chunks[1];
//    chunks[5] = chunks[6];
//    chunks[10] = chunks[11];
//    chunks[15] = chunks[16];
//    chunks[20] = chunks[21];
//
//    chunks[1] = chunks[2];
//    chunks[6] = chunks[7];
//    chunks[11] = chunks[12];
//    chunks[16] = chunks[17];
//    chunks[21] = chunks[22];
//
//    chunks[2] = chunks[3];
//    chunks[7] = chunks[8];
//    chunks[12] = chunks[13];
//    chunks[17] = chunks[18];
//    chunks[22] = chunks[23];
//
//    chunks[3] = chunks[4];
//    chunks[8] = chunks[9];
//    chunks[13] = chunks[14];
//    chunks[18] = chunks[19];
//    chunks[23] = chunks[24];
//
//    GenerateChunk(4 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[4] = new Chunk(noiseVector);
//
//    GenerateChunk(4 - chunkOffsetX, 1 + chunkOffsetY);
//    chunks[9] = new Chunk(noiseVector);
//
//    GenerateChunk(4 - chunkOffsetX, 2 + chunkOffsetY);
//    chunks[14] = new Chunk(noiseVector);
//
//    GenerateChunk(4 - chunkOffsetX, 3 + chunkOffsetY);
//    chunks[19] = new Chunk(noiseVector);
//
//    GenerateChunk(4 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[24] = new Chunk(noiseVector);
//}
//
//void ShiftChunksToRight(){
//
//    chunkOffsetX++;
//
//    delete(chunks[4]);
//    delete(chunks[9]);
//    delete(chunks[14]);
//    delete(chunks[19]);
//    delete(chunks[24]);
//
//    chunks[4] = chunks[3];
//    chunks[9] = chunks[8];
//    chunks[14] = chunks[13];
//    chunks[19] = chunks[18];
//    chunks[24] = chunks[23];
//
//    chunks[3] = chunks[2];
//    chunks[8] = chunks[7];
//    chunks[13] = chunks[12];
//    chunks[18] = chunks[17];
//    chunks[23] = chunks[22];
//
//    chunks[2] = chunks[1];
//    chunks[7] = chunks[6];
//    chunks[12] = chunks[11];
//    chunks[17] = chunks[16];
//    chunks[22] = chunks[21];
//
//    chunks[1] = chunks[0];
//    chunks[6] = chunks[5];
//    chunks[11] = chunks[10];
//    chunks[16] = chunks[15];
//    chunks[21] = chunks[20];
//
//    GenerateChunk(0 - chunkOffsetX, 0 + chunkOffsetY);
//    chunks[0] = new Chunk(noiseVector);
//
//    GenerateChunk(0 - chunkOffsetX, 1 + chunkOffsetY);
//    chunks[5] = new Chunk(noiseVector);
//
//    GenerateChunk(0 - chunkOffsetX, 2 + chunkOffsetY);
//    chunks[10] = new Chunk(noiseVector);
//
//    GenerateChunk(0 - chunkOffsetX, 3 + chunkOffsetY);
//    chunks[15] = new Chunk(noiseVector);
//
//    GenerateChunk(0 - chunkOffsetX, 4 + chunkOffsetY);
//    chunks[20] = new Chunk(noiseVector);
//}

int main() {

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

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cerr << "Failed to load glad\n";
    }

    glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);

    if(glfwExtensionSupported("GL_ARB_multisample")){
        glEnable(GL_MULTISAMPLE);
    }else{
        std::cerr << "Multisampling not supported" << std::endl;
    }

    mainShader = new Shader("shaders/chunkVertex.glsl", "shaders/chunkFragment.glsl");

    mainShader->Use();

    TerrainGenerator terrainGenerator;

//    Texture atlas("atlas2.png"); //33333333333333333333333333333333333333333333333333
    Texture atlas("atlas3.png"); //33333333333333333333333333333333333333333333333333

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

    mainShader->AddUniform("chunkVector");
    mainShader->AddUniform("playerVector");

    glActiveTexture(GL_TEXTURE0);
    atlas.Bind();

    glClearColor(0.4431372549f, 0.6705882353f, 0.6392156863f, 1.0f);
//    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec2 positionMatrix(1.0f);

    double lastTime = glfwGetTime();
    double currentTime;
    double deltaTime;

    int counterChunks = 0;
    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            chunks[counterChunks++] = new Chunk(terrainGenerator.getChunk(j, i, TerrainGenerator::GenerationType::Biomes));
        }
    }

    firstLayer = new RenderLayer(chunks, terrainGenerator, TerrainGenerator::GenerationType::Biomes);

    counterChunks = 0;

    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            chunks[counterChunks++] = new Chunk(terrainGenerator.getChunk(j, i, TerrainGenerator::GenerationType::Decals));
        }
    }

    secondLayer = new RenderLayer(chunks, terrainGenerator, TerrainGenerator::GenerationType::Decals);

    while(!glfwWindowShouldClose(window)){

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        playerPositionVector.x += deltaTime * playerPositionX * moveSpeed;
        playerPositionVector.y += deltaTime * playerPositionY * moveSpeed;

        if(playerPositionVector.x >= 3.2){
            playerPositionVector.x = 0;
            firstLayer->ShiftChunksToRight();
            secondLayer->ShiftChunksToRight();
        }else if(playerPositionVector.x <= -3.2){
            playerPositionVector.x = 0;
            firstLayer->ShiftChunksToLeft();
            secondLayer->ShiftChunksToLeft();
        }else if(playerPositionVector.y >= 3.2){
            playerPositionVector.y = 0;
            firstLayer->ShiftChunksToUp();
            secondLayer->ShiftChunksToUp();
        }else if(playerPositionVector.y <= -3.2){
            playerPositionVector.y = 0;
            firstLayer->ShiftChunksToDown();
            secondLayer->ShiftChunksToDown();
        }

        mainShader->SetUniform("playerVector", playerPositionVector);

        firstLayer->DrawLayer(mainShader);
        secondLayer->DrawLayer(mainShader);

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

//    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)[[]]{
//        myNoise.SetSeed(myNoise.GetSeed() + 1);
//        ShiftChunksToDown();
//        ShiftChunksToDown();
//        ShiftChunksToDown();
//        ShiftChunksToDown();
//        ShiftChunksToDown();
//        ShiftChunksToUp();
//        ShiftChunksToUp();
//        ShiftChunksToUp();
//        ShiftChunksToUp();
//        ShiftChunksToUp();
//    }

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
        moveSpeed = 20.0f;
    }

    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
        moveSpeed = 1.0f;
    }

    if(key == GLFW_KEY_UP && action == GLFW_PRESS){
        firstLayer->ShiftChunksToUp();
        secondLayer->ShiftChunksToUp();
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        firstLayer->ShiftChunksToDown();
        secondLayer->ShiftChunksToDown();
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        firstLayer->ShiftChunksToLeft();
        secondLayer->ShiftChunksToLeft();
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        firstLayer->ShiftChunksToRight();
        secondLayer->ShiftChunksToRight();
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
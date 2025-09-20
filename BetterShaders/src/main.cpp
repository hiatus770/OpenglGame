#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

const int SRC_WIDTH = 1920;
const int SRC_HEIGHT = 1080;
const int CHUNK_SIZE = 500;
const int STARS_PER_CHUNK = 10000;
const int PLANETS_PER_CHUNK = 100;

#include "camera.h"

// TODO
// - make the stars into asteroids that you can interact with and fly around? do i actually want to make a game or whatever sob :sob:  planets make more sense in this case but whats the point in having 300000 of them :sob:
// Projectiles? Questions mark??? COnfusion??? and then maybe implement movement with the mouse so it is actually playable buster :sob:
// - Actually add coloring and possibly textured things please pretty please please
// Make small UI stuff now :sob:
// Pplanet chunking is next, make sure they chunk normally, then add different planet types? and then uhhh idk, make them interactive somehow :wink:
// CLEAN UP CLEAN UP
// Make the update() function for the palyer not in processInput anymore cause its really annoying in there for future use :pensive:
// Fix planet rendering please
    // - Angles aren't working properly, they dont rotate correctly, once done make the chunk renderer actually work please pretty please wahoo

// Sprit needed for player class
std::vector<float> shipSprite = {
    0.0f, 0.5f, 0.0f,
    0.75f, 0.0f, 0.5f,

    0.75f, 0.0f, 0.5f,
    0.0f, -0.25f, 0.0f,

    0.0f, 0.5f, 0.0f,
    -0.75f, 0.0f, 0.5f,

    -0.75f, 0.0f, 0.5f,
    0.0f, -0.25f, 0.0f, // BACK OF SHIP

    // FRONT OF SHIP
    0.0f, 0.0f, -2.0f,
    -0.75f, 0.0f, 0.5f,

    0.0f, -0.25f, 0.0f,
    0.0f, 0.0f, -2.0f,

    0.0f, 0.5f, 0.0f,
    0.0f, 0.0f, -2.0f,

    0.75f, 0.0f, 0.5f,
    0.0f, 0.0f, -2.0f,

    // Back line
    0.0f, -0.25, 0.0f,
    0.0f, 0.5f, 0.0f};

Camera camera; // Global Camera for the entire code thing :)

#include "player.h"
#include "star.h"
#include "starChunk.h"
#include "planet.h"
// #include "planetChunk.h"

// Whenever the window is changed this function is called
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// Mouse call back
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
// Processing all input here
void processInput(GLFWwindow *window);

// Defining our vertices
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

Player player(shipSprite);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    std::cout << "Making Window!" << std::endl;

    // ------------ OPENGL INTIALIZATION ----------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // The major version so x.x the first x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // The minor version of GLFW we are using so x.x the second x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // This means we do not use any backwards compatible features, so its a smaller set of all of OPENGL

    // Creating the window object
    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Moverment", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load GLAD function pointers so that we use the correct openGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OPENGL INITIALIZATION
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // The global shader for the program
    Shader globalShader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs");

    // The test object
    Object object(&globalShader, shipSprite, {1.0, 1.0, 1.0, 1.0});

    // Create player object
    player.createPlayerObject(&globalShader);

    Star cameraStar(&globalShader, glm::vec3(1.0f, 1.0f, 1.0f));
    Star directionStar(&globalShader, glm::vec3(1.0f, 1.0f, 1.0f));

    StarChunk *localChunks[7];
    localChunks[0] = new StarChunk(0,0,0);   // 0,0
    localChunks[1] = new StarChunk(0,0,0);   // +x
    localChunks[2] = new StarChunk(0,0,0);   // -x
    localChunks[3] = new StarChunk(0,0,0);   // +z
    localChunks[4] = new StarChunk(0,0,0);   // -z
    localChunks[5] = new StarChunk(0,0,0);   // +y
    localChunks[6] = new StarChunk(0,0,0);   // -y
    for(int i = 0; i < 7; i++){
        localChunks[i]->init(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    Planet planet(&globalShader, glm::vec3(4.0f, 4.0f, 4.0f), {1.0f, 0.0f, 0.0f});
    planet.rotAxis = glm::vec3(1.0f, 1.0f, 0.0f);

    std::vector<Planet> planets;

    // for(int i = 0; i < 100; i++){
        // Planet newPlanet(&globalShader, glm::vec3(rand()%300, rand()%300, rand()%300),glm::normalize(glm::vec3(rand()%300, rand()%300, rand()%300)), {0.0f, 1.0f, 1.0f});
        // planets.push_back(newPlanet);

    // }

    glm::vec3 playerChunkCoords(1.0f, 1.0f, 1.0f);
    glm::vec3 lastPlayerChunkCoords(2.0f, 1.0f, 1.0f);

    // Main Loop of the function
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        std::cout << "FPS: " << 1 / deltaTime << std::endl;

        // Clear the screen before we start
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process input call
        processInput(window);

        // Calculate Player chunk coord difference
        playerChunkCoords.x = floor(player.position.x / CHUNK_SIZE);
        playerChunkCoords.y = floor(player.position.y / CHUNK_SIZE);
        playerChunkCoords.z = floor(player.position.z / CHUNK_SIZE);

        if (playerChunkCoords.x - lastPlayerChunkCoords.x != 0 || playerChunkCoords.z != lastPlayerChunkCoords.z || playerChunkCoords.y != lastPlayerChunkCoords.y){
            // We changed position between frames make new StarChunk
            // chunk->init(playerChunkCoords);
            localChunks[0]->init(playerChunkCoords + glm::vec3{0.0, 0.0, 0.0});
            localChunks[1]->init(playerChunkCoords + glm::vec3{1.0, 0.0, 0.0});
            localChunks[2]->init(playerChunkCoords + glm::vec3{-1.0, 0.0, 0.0});
            localChunks[3]->init(playerChunkCoords + glm::vec3{0.0, 0.0, 1.0});
            localChunks[4]->init(playerChunkCoords + glm::vec3{0.0, 0.0, -1.0});
            localChunks[5]->init(playerChunkCoords + glm::vec3{0.0, 1.0, 0.0});
            localChunks[6]->init(playerChunkCoords + glm::vec3{0.0, -1.0, 0.0});
        }

        globalShader.setVec3("cameraPos", {player.getCameraPosition().x, player.getCameraPosition().y, player.getCameraPosition().z});

        // Debug purposes for showing where the camera is!
        cameraStar.position = player.getCameraPosition();
        cameraStar.render(0.1, camera.getViewMatrix(), camera.getProjectionMatrix());

        // All render calls should go here!
        player.render();
        for(int i = 0; i < 7; i++){
            localChunks[i]->render();
        }
        for(int i = 0; i < planets.size(); i++){
            planets[i].render(0.1, camera.getViewMatrix(), camera.getProjectionMatrix());
        }
        planet.render(0.1, camera.getViewMatrix(), camera.getProjectionMatrix());

        lastPlayerChunkCoords = playerChunkCoords;

        glfwSwapBuffers(window); // Swaps the color buffer that is used to render to during this render iteration and show it ot the output screen
        glfwPollEvents();        // Checks if any events are triggered, updates the window state andcalls the corresponding functions
    }

    glfwTerminate(); // Call this function to properly clean up GLFW's big mess
    return 0;
}

// This function is going to be used to resize the viewport everytime it is resized by the user
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Currently no mouse controls are enabled
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // float xpos = static_cast<float>(xposIn);
    // float ypos = static_cast<float>(yposIn);
    // if (firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;
    // lastX = xpos;
    // lastY = ypos;

    // float sensitivity = 0.1f;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // yaw   += xoffset;
    // pitch += yoffset;

    // if(pitch > 89.0f)
    //     pitch = 89.0f;
    // if(pitch < -89.0f)
    //     pitch = -89.0f;

    // glm::vec3 direction;
    // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // direction.y = sin(glm::radians(pitch));
    // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(direction);
}

/**
 * @brief Handles all user input given the window object, currently handles player movement
 *
 * @param window
 */
void processInput(GLFWwindow *window)
{

    // Player ENUMS ARE
    /*
    FORWARD,
    BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT,
    RISE,
    FALL,
    PITCH_UP,
    PITCH_DOWN,
    YAW_LEFT,
    YAW_RIGHT
    */

    // Function is used as follows player.processKeyboard(ENUM, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.processKeyboard(STRAFE_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.processKeyboard(STRAFE_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        player.processKeyboard(PITCH_UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player.processKeyboard(PITCH_DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player.processKeyboard(YAW_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player.processKeyboard(YAW_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        player.processKeyboard(RISE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        player.processKeyboard(FALL, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        player.processKeyboard(ROLL_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        player.processKeyboard(ROLL_LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        /// Reset orientation
        player.direction = glm::vec3(1.0f, 0.0f, 0.0f);
        player.localUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // Update calls
    player.update();

    camera.position = player.getCameraPosition();
    camera.direction = player.getCameraDirection();
    camera.cameraUp = player.getCameraUp();
    camera.projection =  glm::perspective(glm::radians(60.0f), (float)SRC_WIDTH/SRC_HEIGHT, 0.1f, 1000.0f);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // Global camera mode
        camera.position = player.position + glm::vec3(20.0f, 20.0f, 20.0f);
        camera.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
        camera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        camera.projection =  glm::perspective(glm::radians(90.0f), (float)SRC_WIDTH/SRC_HEIGHT, 0.1f, 100.0f);
    }


}

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

const int SRC_WIDTH = 600;
const int SRC_HEIGHT = 600;

#include "camera.h"

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

// Player player({
//     // Shaft of the arrow
//     0.0f,  0.0f,  0.0f,  // Start point
//     0.0f,  0.0f, -1.0f,  // End point

//     // First line of the arrowhead
//     0.0f,  0.0f, -1.0f,  // Start point
//    -0.25f,  0.25f, 0.5f,  // End point

//     // Second line of the arrowhead
//     0.0f,  0.0f, -1.0f,  // Start point
//     0.25f,  0.25f, 0.5f   // End point
// });

// Player player({
//     0.1f, 0.0f, -0.1f,
//     0.0f, 0.0f, 0.0f,

//     -0.1f, 0.0f, -0.1f,
//     0.0f, 0.0f, 0.0f,

//     0.0f, 0.1f, -0.2f,
//     0.0f, 0.0f, 0.0f
// });

Player player(shipSprite);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float roll = 0.0f;
float lastX = SRC_WIDTH / 2.0;
float lastY = SRC_HEIGHT / 2.0;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    std::cout << "Making Window!" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // The major version so x.x the first x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // The minor version of GLFW we are using so x.x the second x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // This means we do not use any backwards compatible features, so its a smaller set of all of OPENGL

    // Creating the window object
    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Moverment", NULL, NULL);

    // If it wasn't created then we stop here
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

    // Set the frameBufferSizeballback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    Shader globalShader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs");

    Object object(&globalShader, shipSprite, {1.0, 1.0, 1.0, 1.0});

    // Make a vector of star objects
    unsigned int amount = 10000;
    std::vector<glm::vec3> stars; 

    Shader starShader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vertInstanced.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/fragInstanced.fs");

    for (int i = 0; i < 10000; i++)
    {
        glm::vec3 position; 
        position.x = i;
        position.y = i % 3;
        position.z = 2 * i % 5 * sin(glfwGetTime() * i);
        stars.push_back(position); 
    }

    unsigned int VBO; 
    unsigned int VAO; 
    unsigned int instanceVBO; 

    glGenBuffers(1, &instanceVBO); 
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stars.size(), stars.data(), GL_STATIC_DRAW); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO); // Generate 1

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * starVertices.size(), starVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); 
    glEnableVertexAttribArray(1);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glVertexAttribDivisor(1, 1); 

    player.createPlayerObject(&globalShader);

    Star cameraStar(&globalShader, glm::vec3(1.0f, 1.0f, 1.0f));
    Star directionStar(&globalShader, glm::vec3(1.0f, 1.0f, 1.0f));

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

        object.matrixTransform(glm::rotate(object.model, glm::radians(1.001f), glm::vec3(0.0f, 0.0f, 1.0f)));
        object.matrixTransform(glm::rotate(object.model, glm::radians(3.001f), glm::vec3(0.0f, 1.0f, 0.0f)));

        // // SLOW ALERT!!! YOWZA
        // for (Star star : stars)
        // {
        //     star.render(deltaTime, camera.getViewMatrix(), camera.getProjectionMatrix());
        // }
        starShader.use();
        starShader.setVec4("aColor", {1.0f, 0.0f, 0.0f, 1.0f}); 
        starShader.setMat4("projection", camera.getProjectionMatrix());
        starShader.setMat4("view", camera.getViewMatrix()); 
        starShader.setMat4("model", glm::mat4(1.0f)); 
        glBindVertexArray(VBO); 
        glDrawArraysInstanced(GL_LINES, 0, starVertices.size(), amount); 
        glBindVertexArray(0); 

        cameraStar.position = player.getCameraPosition();
        cameraStar.render(0.1, camera.getViewMatrix(), camera.getProjectionMatrix());

        player.render();
        object.render(camera.getViewMatrix(), camera.getProjectionMatrix(), GL_LINES);

        // texture.transform = glm::rotate(texture.transform, glm::radians(10.0f), glm::vec3(1.0f, 2.5f, 0.0f));
        // texture.render();

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

    camera.position = player.getCameraPosition();
    camera.direction = player.getCameraDirection();
    camera.cameraUp = player.getCameraUp();

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // Global camera mode
        camera.position = player.position + glm::vec3(10.0f, 10.0f, 10.f);
        camera.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
        camera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

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
    0.75f, 0.0f, -0.5f,

    0.75f, 0.0f, -0.5f,
    0.0f, -0.25f, 0.0f,

    0.0f, 0.5f, 0.0f,
    -0.75f, 0.0f, -0.5f,

    -0.75f, 0.0f, -0.5f,
    0.0f, -0.25f, 0.0f, // BACK OF SHIP

    // FRONT OF SHIP
    0.0f, 0.0f, 1.0f,
    -0.75f, 0.0f, -0.5f,

    0.0f, -0.25f, 0.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.5f, 0.0f,
    0.0f, 0.0f, 1.0f,

    0.75f, 0.0f, -0.5f,
    0.0f, 0.0f, 1.0f};


Camera camera; // Global Camera for the entire code thing :)  

#include "player.h"

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

Player player({
    // Shaft of the arrow
    0.0f,  0.0f,  0.0f,  // Start point
    0.0f,  0.0f, -1.0f,  // End point

    // First line of the arrowhead
    0.0f,  0.0f, -1.0f,  // Start point
   -0.25f,  0.25f, 0.5f,  // End point

    // Second line of the arrowhead
    0.0f,  0.0f, -1.0f,  // Start point
    0.25f,  0.25f, 0.5f   // End point
}); 


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



    Object object(shipSprite,
                  {1.0, 1.0, 1.0, 1.0});
    Object object2({-0.2, 1, 0, 0.75, 0.75, 0, 0, -0.75, 0, 0.4, -0.35, 0}, {0, 1.0, 0, 1.0});

    // Create our fragment and vertex shaders
    Shader shader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs");

    Texture texture("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/amogus2.png", {
                                                                                              0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
                                                                                              0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
                                                                                              -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                                                                                              -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
                                                                                              0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
                                                                                              -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f  // bottom left
                                                                                          });



    
    player.createPlayerObject(); 


    // Main Loop of the function
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        // Clear the screen before we start
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process input call
        processInput(window);


        // object.matrixTransform(glm::rotate(object.model, glm::radians(1.001f), glm::vec3(0.0f, 0.0f, 1.0f)));
        object.matrixTransform(glm::rotate(object.model, glm::radians(3.001f), glm::vec3(0.0f, 1.0f, 0.0f)));
        // object.matrixTransform(glm::rotate(object.model, glm::radians(-1.001f), glm::vec3(1.0f, 0.5f, 1.0f)));


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
        player.processKeyboard(ROLL_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        player.processKeyboard(ROLL_RIGHT, deltaTime);

    camera.position = player.getCameraPosition();
    camera.direction = player.getCameraDirection(); 
    camera.cameraUp = player.getCameraUp(); 

    // const float cameraSpeed = 0.05f; // adjust accordingly
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.position += cameraSpeed * camera.direction;
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.position -= cameraSpeed * camera.direction;
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.cameraUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), camera.direction)) * camera.cameraUp;
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.cameraUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), camera.direction)) * camera.cameraUp;
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    // {
    //     camera.direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::cross(camera.direction, camera.cameraUp)) * glm::vec4(camera.direction, 1.0f));
    // }
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    // {
    //     camera.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::cross(camera.direction, camera.cameraUp))) * camera.direction;
    // }

    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    // {
    //     camera.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), camera.cameraUp)) * camera.direction;
    // }

    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    // {
    //     camera.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), camera.cameraUp)) * camera.direction;
    // }

    // // Strafing keys
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    // {
    //     camera.position -= glm::normalize(glm::cross(camera.direction, camera.cameraUp)) * cameraSpeed;
    // } 
    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    // {    
    //     camera.position += glm::normalize(glm::cross(camera.direction, camera.cameraUp)) * cameraSpeed;
    // }

    // // Rise and Fall keys
    // if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    // {
    //     camera.position += glm::normalize(camera.cameraUp) * cameraSpeed;
    // } 
    // if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    // {    
    //     camera.position -= glm::normalize(camera.cameraUp) * cameraSpeed;
    // }

}

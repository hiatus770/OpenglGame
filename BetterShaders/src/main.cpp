#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h" 
#include "object.h" 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h" 

const int SRC_WIDTH = 600;
const int SRC_HEIGHT = 600;

// Whenever the window is changed this function is called
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// Processing all input here
void processInput(GLFWwindow *window);

// Defining our vertices
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

int main()
{
    std::cout << "Making Window!" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // The major version so x.x the first x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // The minor version of GLFW we are using so x.x the second x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // This means we do not use any backwards compatible features, so its a smaller set of all of OPENGL

    // Creating the window object
    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Better Shaders", NULL, NULL);


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


    Object object({-0.75, -0.75, 0, 0.75, 0.75, 0, 0, -0.75, 0, 0.1, 0.35, 0}, {1.0, 0, 0, 1.0}); 
    Object object2({-0.2, 1, 0, 0.75, 0.75, 0, 0, -0.75, 0, 0.4, -0.35, 0}, {0, 1.0, 0, 1.0});

    // Create our fragment and vertex shaders 
    Shader shader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs");
 
    Texture texture("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/amogus.png", {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left  
         0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f // bottom left
    });

    // VBO STUFF
    unsigned int VBO;
    glGenBuffers(1, &VBO);                                                     // Create the buffer for the VBO

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    // Main Loop of the function
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen before we start
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Process input call
        processInput(window);
        
        
        shader.use(); 
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,3); 

        
        object.render(); 
        object2.render(GL_LINE_STRIP); 

        texture.render(); 

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

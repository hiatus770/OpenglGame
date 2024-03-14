#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// This function is going to be used to resize the viewport everytime it is resized by the user 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 
}

int main()
{
    std::cout << "Making Window!" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // The major version so x.x the first x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // The minor version of GLFW we are using so x.x the second x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // This means we do not use any backwards compatible features, so its a smaller set of all of OPENGL

    // Creating the window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "Window!", NULL, NULL);

    // IF it wasn't created then we stop here
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

    // Viewport
    glViewport(0,0, 800, 600); 

    // Set the frameBufferSizeballback function 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    // Main Loop of the function
    while(!glfwWindowShouldClose(window)){
        // Process input call
        processInput(window); 

        glfwSwapBuffers(window); // Swaps the color buffer that is used to render to during this render iteration and show it ot the output screen 
        glfwPollEvents(); // Checks if any events are triggered, updates the window state andcalls the corresponding functions 
    }


    glfwTerminate(); // Call this function to properly clean up GLFW's big mess 
    return 0;
}
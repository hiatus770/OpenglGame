#ifndef STAR_H 
#define STAR_H


#include <vector>
#include "shader.h"
#include "object.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::vector<float> starVertices = {
    // Positions
    0.0f, 0.1f, 0.0f,     // Top point
    0.0f, -0.1f, 0.0f,    // Left point
    
    0.1f, 0.0f, 0.0f,     // Right point
    -0.1f, 0.0f, 0.0f,   // Bottom left point

    0.0f, 0.0f, 0.1f,    // Bottom right point
    0.0f, 0.0f, -0.1f,    // Top point (back)
};

class Star{
public: 
    // Only takes in 3 parameters, the x y and z coordiantes and thats it :) 
    Object* starObj; 
    glm::vec3 position; 
    std::vector<float> color; 
    glm::vec3 rotAxis = glm::vec3(0.0f, 1.0f, 0.0f); 

    Star(glm::vec3 Position, std::vector<float> Color = {1.0f, 1.0f, 1.0f, 1.0f}){
        position = Position; 
        color = Color; 
        starObj = new Object(starVertices, Color); 
    }    

    // Render the star
    void render(float deltaTime, glm::mat4 view, glm::mat4 projection){ 
        color[1] = (float)sin(glfwGetTime()); 
        starObj->objColor = color; 
        starObj->model = (glm::translate(glm::mat4(1.0f), position));  
        starObj->render(view, projection); 
    }  

    

};



#endif 

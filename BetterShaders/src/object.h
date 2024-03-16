#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include "shader.h" 
#include <vector>
#include <iostream> 

class Object
{
public:
    std::vector<float> vertices; // The list of vertices for all the lines
    unsigned int VBO; 
    unsigned int VAO; 
    std::vector<float> objColor; 
    glm::mat4 transform = glm::mat4(1.0f); 

    Shader* shader; 

    Object(std::vector<float> v, 
           std::vector<float> color = {1.0, 1.0, 1.0, 1.0}, 
           std::string vertexShaderPath = "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", 
           std::string fragmentShaderPath = "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs")
    {
        if (v.size()%6 != 0){
            std::cout << "Invalid size for vertices, must be pairs of 3 floats\n"; 
            return; 
        }

        for(int i = 0; i < v.size(); i++){
            vertices.push_back(v[i]); 
        }

        for(int i = 0; i < color.size(); i++){
            objColor.push_back(color[i]); 
        }

        shader = new Shader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vert.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/frag.fs");


        glGenBuffers(1, &VBO); 
        glGenVertexArrays(1, &VAO); // Generate 1 

        glBindVertexArray(VAO); 
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW); 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0);


    }


    void matrixTransform(glm::mat4 transformation){
        // transform = transform * transformation; 
        transform = transformation; 
        shader->use(); 
        shader->setMat4("transform", transform); 
    }

    void render(GLenum mode = GL_LINES){
        shader->use(); 
        shader->setVec4("color", objColor); 
        glBindVertexArray(VAO); 
        glDrawArrays(mode, 0, vertices.size()); 
    }

};
#endif 
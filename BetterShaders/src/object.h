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

    Shader* shader; 

    Object(std::vector<float> v, std::vector<float> color = {1.0, 1.0, 1.0, 1.0}){
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

    void render(){
        shader->use(); 
        shader->setVec4("color", objColor); 
        glBindVertexArray(VAO); 
        glDrawArrays(GL_LINES, 0, vertices.size()); 
    }

};
#endif 